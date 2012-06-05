
#include <common/inc/MyDebug.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/data_proxy/DataProxyIf.h>
#include <interface/service_group/ServiceGroupIf.h>
#include <interface/protocol/TlvIf.h>
#include <interface/system/SystemInfoIf.h>

#include <data_proxy/net/inc/NetDataProxy.h>

////////////////////////////////////////////////////////////////////////////////
// DataProxyIf
////////////////////////////////////////////////////////////////////////////////
response_handler_callback funcRspHandlerCB_m = NULL;

void SET_CLIENT_RESPONSE_HANDLER(response_handler_callback func)
{
    funcRspHandlerCB_m = func;
}

void SEND_NET_REQUEST(int nSgId, const char* pBuf, int nLen)
{
    NetDataProxy_c::getInstance()->insertRequest(nSgId, pBuf, nLen);
}

////////////////////////////////////////////////////////////////////////////////
// NetDataNode_c
////////////////////////////////////////////////////////////////////////////////
NetDataNode_c::NetDataNode_c() :
    nSgId_m(0),
    pIoTrans_m(NULL),
    nAppendingIndex_m(0)
{
}

NetDataNode_c::~NetDataNode_c()
{
    releaseIoTrans();
}

void NetDataNode_c::appendRequest(const char* pReq, int nLen)
{
    if ((NULL == pReq) || (0 >= nLen))
    {
        return;
    }

    // append header
    if (0 >= tbBuffer_m[nAppendingIndex_m].getLength())
    {
        TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(1);

        // 1. protocol version
        TlvAttrIf_i* tlvVersion = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
        tlvVersion->setValue_int32(0x00010000);
        lstAttrs.append(tlvVersion);

        // encode message content
        TlvEncoder_c tlvEncoder;
        tlvEncoder.generate(&lstAttrs, tbBuffer_m[nAppendingIndex_m]);

        // release tlv attributes
        for (int nIndex=lstAttrs.size()-1; nIndex>=0; nIndex--)
        {
            delete lstAttrs.takeLast();
        }
    }

    tbBuffer_m[nAppendingIndex_m].append(pReq, nLen);
}

void NetDataNode_c::start()
{
    // if io transaction has been working, return first and wait for next round
    if (isIOWorking())
    {
        return;
    }

    allocateIoTrans();
}

void NetDataNode_c::allocateIoTrans()
{
    const char* sHost = NULL;
    const char* sPort = NULL;

    // query service group info
    if (nSgId_m > 0)
    {
        if (!SERVICE_GROUP_INFO_QUERY(nSgId_m, sHost, sPort) || (NULL == sHost) || (NULL == sPort))
        {
            return;
        }
    }
    else
    {
        // message to dispatcher
        sHost = SYS_INFO_GET_STRING(SYS_INFO_TYPE_DI_HOST);
        sPort = SYS_INFO_GET_STRING(SYS_INFO_TYPE_DI_PORT);
    }

    pIoTrans_m = IO_TRANSACTION_ALLOC(this, sHost, sPort); 
    if (NULL != pIoTrans_m)
    {
        //pIoTrans_m->setIoData(this);
    }
}

void NetDataNode_c::releaseIoTrans()
{
    if (NULL == pIoTrans_m)
    {
        return;
    }

    IO_TRANSACTION_RELEASE(pIoTrans_m);
    pIoTrans_m = NULL;
}

bool NetDataNode_c::hasNewData()
{
    return (tbBuffer_m[nAppendingIndex_m].getLength() > 0);
}

bool NetDataNode_c::isIOWorking()
{
    return (NULL != pIoTrans_m);
}

TempMemBuffer_c* NetDataNode_c::retrieveRequestContent()
{
    // append tail flag
    if (0 < tbBuffer_m[nAppendingIndex_m].getLength())
    {
        TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(1);

        // 1. protocol version
        TlvAttrIf_i* tlvTail = createTlvAttribute(TLV_ATTR_TAIL_FLAG);
        tlvTail->setValue_int32(0);
        lstAttrs.append(tlvTail);

        // encode message content
        TlvEncoder_c tlvEncoder;
        tlvEncoder.generate(&lstAttrs, tbBuffer_m[nAppendingIndex_m]);

        // release tlv attributes
        for (int nIndex=lstAttrs.size()-1; nIndex>=0; nIndex--)
        {
            delete lstAttrs.takeLast();
        }
    }

    nAppendingIndex_m = 1 - nAppendingIndex_m;
    tbBuffer_m[nAppendingIndex_m].clear();

    return &tbBuffer_m[1 - nAppendingIndex_m];
}

void NetDataNode_c::analyzeResponse(TempMemBuffer_c& tbBuffer)
{
    //tbBuffer.cutAsString();
    MSG("RECEIVED reponse, to analyze...\n");
    TempSimpleVector_c<TlvAttrIf_i*> lstResponseAttrs(8);

    TlvParser_c parser(&lstResponseAttrs);
    parser.parse(tbBuffer.getBuffer(), tbBuffer.getLength());

    MSG("returned %d attributes\n", lstResponseAttrs.size());

    //processSGResponse(&lstResponseAttrs);
    (*funcRspHandlerCB_m)(&lstResponseAttrs);

    // TODO: to suppot persistant connection in the future
    MSG("release io trans...\n");
    releaseIoTrans();

    ////to check if need to start new round checking
    //if (NULL != lstPendingGroup_m) // && (lstPendingGroup_m->size() > 0))
    //{
    //    sendSGCheckEvent();
    //}
    if (!hasNewData())
    {
        NetDataProxy_c::getInstance()->releaseNode(nSgId_m);
    }
    else
    {
        // allocate new Io trasaction
        allocateIoTrans();
    }
}

////////////////////////////////////////////////////////////////////////////////
// NetDataProxy_c
////////////////////////////////////////////////////////////////////////////////
NetDataProxy_c::NetDataProxy_c() :
    lstIdleNodes_m(8)
{
}

NetDataProxy_c::~NetDataProxy_c()
{
    deleteIdleNodes();
}

IMPLE_OBJECT_GET_INSTANCE(NetDataProxy_c)

bool NetDataProxy_c::insertRequest(int nSgId, const char* pBuf, int nLen)
{
    NetDataNode_c* pNode = htNodes_m.lookup(nSgId);
    if (NULL == pNode)
    {
        pNode = new NetDataNode_c();
        if (NULL == pNode)
        {
            return false;
        }

        pNode->setSgId(nSgId);

        htNodes_m.insert(pNode->getIdAddress(), pNode);
    }

    pNode->appendRequest(pBuf, nLen);
    pNode->start();

    return true;
}

void NetDataProxy_c::releaseNode(int nSgId)
{
    NetDataNode_c* pNode = htNodes_m.remove(nSgId);
    if (NULL == pNode)
    {
        return;
    }

    lstIdleNodes_m.append(pNode);
}

void NetDataProxy_c::deleteIdleNodes()
{
    for (int nIndex=lstIdleNodes_m.size()-1; nIndex>=0; nIndex--)
    {
        delete lstIdleNodes_m.takeLast();
    }
}

