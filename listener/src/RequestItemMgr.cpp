#include <common/inc/MyDebug.h>

#include <interface/baseos/TempMemBuffer.h>
#include <interface/protocol/TlvIf.h>
#include <interface/listener/ListenerIf.h>
#include <interface/system/GlobalInstanceMgr.h>

#include <listener/inc/RequestItem.h>
#include <listener/inc/RequestItemMgr.h>

////////////////////////////////////////////////////////////////////////////////
// ListenerIf
////////////////////////////////////////////////////////////////////////////////
void SET_SERVER_REQUEST_HANDLER(request_handler_callback func)
{
    RequestItemMgr_c::getInstance()->setRequestHandler(func);
}

////////////////////////////////////////////////////////////////////////////////
// Request Item Mgr
////////////////////////////////////////////////////////////////////////////////
RequestItemMgr_c::RequestItemMgr_c() :
    funcReqHandlerCB_m(NULL)
{
}

RequestItemMgr_c::~RequestItemMgr_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(RequestItemMgr_c)

RequestItem_c* RequestItemMgr_c::allocateReqItem()
{
    if (lstReqItemObj_m.size() > 0)
    {
        RequestItem_c* pItem = lstReqItemObj_m.takeLast();
        pItem->init();

        return pItem;
    }
    else
    {
        return new RequestItem_c();
    }
}

void RequestItemMgr_c::releaseReqItem(RequestItem_c* pReqItem)
{
    if (NULL == pReqItem)
    {
        return;
    }

    lstReqItemObj_m.append(pReqItem);
}

void RequestItemMgr_c::releaseReqItem(int nSocketFd)
{
    RequestItem_c* pIdleItem = htRequestItems_m.remove(nSocketFd);
    if (NULL == pIdleItem)
    {
        MSG_ERR("could not find request item with fd [%d] in table\n", nSocketFd);
    }

    releaseReqItem(pIdleItem);
}

bool RequestItemMgr_c::handleRequest(int nFd, TempMemBuffer_c& dataReceived)
{
    //MSG("[%d] data received from client, to analyze...\n", dataReceived.getLength());

    bool bRet = true;

    bool bNewRequester = false;

    RequestItem_c* pReqItem = htRequestItems_m.lookup(nFd);
    if (NULL == pReqItem)
    {
        pReqItem = allocateReqItem();
        if (NULL == pReqItem)
        {
            return false;
        }

        pReqItem->setSocketFd(nFd);
        bNewRequester = true;

        // insert into hash table
        htRequestItems_m.insert(pReqItem->getFdAddress(), pReqItem);
    }


    if (!bNewRequester)
    {
        pReqItem->appendBuffer(dataReceived.getBuffer(), dataReceived.getLength());
        if (TlvParser_c::isMessageCompleted(pReqItem->getRequest(), pReqItem->getLength()))
        {
            bRet= processRequest(pReqItem, pReqItem->getRequest(), pReqItem->getLength());
        }
    }
    else
    {
        if (TlvParser_c::isMessageCompleted(dataReceived.getBuffer(), dataReceived.getLength()))
        {
            bRet = processRequest(pReqItem, dataReceived.getBuffer(), dataReceived.getLength());
        }
        else
        {
            pReqItem->appendBuffer(dataReceived.getBuffer(), dataReceived.getLength());
        }
    }

    return bRet;
}

bool RequestItemMgr_c::processRequest(RequestItem_c* pItem, const char* pRequest, int nLen)
{
    // full message is ready, don't need push into hash table 
    TempSimpleVector_c<TlvAttrIf_i*> lstDecodedAttrs(8);

    TlvParser_c parser(&lstDecodedAttrs);
    parser.parse(pRequest, nLen);

    if (pItem->getHandler())
    {
        // analyze request 
        pItem->getHandler()->processRequest(&lstDecodedAttrs);
    }
    else
    {
        // new requester, try to get proper handler
        (*funcReqHandlerCB_m)(pItem, &lstDecodedAttrs);

        // if no handler, close connection
        if (NULL == pItem->getHandler())
        {
            releaseReqItem(pItem->getSocketFd());

            IO_SOCKET_CLOSE(pItem->getSocketFd());
        }
    }

    // release tlv attributes
    for (int nIndex=lstDecodedAttrs.size()-1; nIndex>=0; nIndex--)
    {
        delete lstDecodedAttrs.takeLast();
    }

    return true;
}
