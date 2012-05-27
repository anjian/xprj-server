#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>

//#include <interface/system/GlobalInstanceMgr.h>

#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>
#include <interface/listener/RequestHandlerIf.h>
#include <interface/listener/ListenerIf.h>

#include <dispatcher/inc/RequestHandler.h>

#if 0
RequestHandler_c::RequestHandler_c()
{
}

RequestHandler_c::~RequestHandler_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(RequestHandler_c)

bool RequestHandler_c::process(int nFd, TempMemBuffer_c& dataReceived, TempMemBuffer_c& dataToSend)
{
    if (dataReceived.getLength() <= 0)
    {
        return false;
    }

    dataReceived.cutAsString();
    MSG("Received from client [%d]: %s\n", nFd, dataReceived.getBuffer());

    // echo back
    const char* sEcho = "echo back";

    dataToSend.append(sEcho, xstrlen(sEcho));

    return true;
}
#endif
////////////////////////////////////////////////////////////////////////////////
// declare private functions
////////////////////////////////////////////////////////////////////////////////
bool handleVersionAttr(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
bool handleTailAttr(TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
bool handleServiceGroupQuery(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);

bool requestHandler(RequestItemIf_i* pReqItem, TempSimpleVector_c<TlvAttrIf_i*>* pRequest)
{
    if (NULL == pRequest)
    {
        return false;
    }

    MSG("received %d attributes.\n", pRequest->size());

    TempSimpleVector_c<TlvAttrIf_i*> pResponse(8);

    // handle version attribute
    {
        TlvAttrIf_i* pAttr = pRequest->get(0);
        if (TLV_ATTR_CLIENT_VERSION == pAttr->getType())
        {
            handleVersionAttr(pAttr, &pResponse);
        }
        else
        {
            // TODO: error case
        }
    }

    // handle content
    for (int nIndex=1; nIndex < pRequest->size(); nIndex++)
    {
        TlvAttrIf_i* pAttr = pRequest->get(nIndex);
        switch (pAttr->getType())
        {
            case TLV_ATTR_SERVICE_GROUP_QUERY:
                {
                    handleServiceGroupQuery(pAttr, &pResponse);
                }
                break;
            case TLV_ATTR_CLIENT_VERSION:
            case TLV_ATTR_TAIL_FLAG:
                {
                    // TODO: error case
                }
                break;
            default:;
        }
    }

    // append tail 
    handleTailAttr(&pResponse);

    // generate reponse
    MSG("Send back %d attributes.\n", pResponse.size());
    if (pResponse.size() > 0)
    {
        TempMemBuffer_c tbResponse;
        TlvEncoder_c tlvEncoder;
        tlvEncoder.generate(&pResponse, tbResponse);

        IO_SOCKET_WRITE(pReqItem->getSocketFd(), tbResponse);
    }

    return true;
}

bool handleVersionAttr(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    TlvAttrIf_i* tlvVer = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
    if (NULL == tlvVer)
    {
        return false;
    }

    tlvVer->setValue_buffer(pAttr->getValue(), pAttr->getValueLen());
    pResponse->append(tlvVer);

    // TODO: retrive the client version, and the following message handling may depend on it

    return true;
}

bool handleTailAttr(TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    // append tail flag
    TlvAttrIf_i* tlvTail = createTlvAttribute(TLV_ATTR_TAIL_FLAG);
    if (NULL == tlvTail)
    {
        return false;
    }

    tlvTail->setValue_int32(0);
    pResponse->append(tlvTail);

    return true;
}

bool handleServiceGroupQuery(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    TlvAttrIf_i* tlvSg = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_UPDATE);
    if (NULL == tlvSg)
    {
        return false;
    }

    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        delete tlvSg;
        return false;
    }

    int nSgId = -1;

    // get service group
    {
        TlvAttrIf_i* pItem = NULL;
        for (;(pItem = pIterator->next());)
        {
            if (TLV_ATTR_SERVICE_GROUP_ID == pItem->getType())
            {
                nSgId = pItem->getInt32();
                break;
            }
        }
        delete pIterator;

        // could not find service group id
        if (NULL == pItem)
        {
            delete tlvSg;
            return false;
        }
    }

    MSG("Check Service Group Info: %d\n", nSgId);

    // TODO: get service group info
    const char* sHost = "127.0.0.1";
    const char* sPort = "8082";

    bool bCheckSGInfo = true;


    if (!bCheckSGInfo)
    {
        delete tlvSg;
        return false;
    }

    // 1. Sg Id
    {
        TlvAttrIf_i* tlvSgId = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_ID);
        if (NULL == tlvSgId)
        {
            delete tlvSg;
            return false;
        }

        tlvSgId->setValue_int32(nSgId);
        tlvSg->appendAttr(tlvSgId);
    }

    // 2. sg host 
    {
        TlvAttrIf_i* tlvSgHost = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_HOST);
        if (NULL == tlvSgHost)
        {
            delete tlvSg;
            return false;
        }

        tlvSgHost->setValue_buffer(sHost, xstrlen(sHost));
        tlvSg->appendAttr(tlvSgHost);
    }

    // 3. sg port
    {
        TlvAttrIf_i* tlvSgPort = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_PORT);
        if (NULL == tlvSgPort)
        {
            delete tlvSg;
            return false;
        }

        tlvSgPort->setValue_buffer(sPort, xstrlen(sPort));
        tlvSg->appendAttr(tlvSgPort);
    }

    // append item into reponse list
    pResponse->append(tlvSg);

    return true;
}

