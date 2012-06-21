#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>
#include <common/inc/MyTime.h>

//#include <interface/system/GlobalInstanceMgr.h>

#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>
#include <interface/listener/ListenerIf.h>

#include <UserService/inc/UserCenter.h>
#include <UserService/inc/UserInfo.h>
#include <UserService/inc/RequestHandler.h>

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
bool handleVersionAttr(TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
bool handleTailAttr(TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
//bool handleUserConnecting(RequestItemIf_i* pReqItem, TlvAttrIf_i* pAttr);

// user auth request
bool handleUserAuth(RequestItemIf_i* pReqItem, TlvAttrIf_i* pAttr);

// chat info from other service groups
bool handleChatInfo(RequestItemIf_i* pReqItem, TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);

//bool handleServiceGroupQuery(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);

bool clientRequestHandler(RequestItemIf_i* pReqItem, TempSimpleVector_c<TlvAttrIf_i*>* pRequest)
{
    if (NULL == pRequest)
    {
        return false;
    }

    TempSimpleVector_c<TlvAttrIf_i*> pResponse(8);
#if 0
    // handle version attribute
    {
        TlvAttrIf_i* pAttr = pRequest->get(0);
        if (TLV_ATTR_CLIENT_VERSION == pAttr->getType())
        {
            handleVersionAttr(pAttr, pResponse);
        }
        else
        {
            // TODO: error case
        }
    }
#endif

    // handle content
    for (int nIndex=1; nIndex < pRequest->size(); nIndex++)
    {
        TlvAttrIf_i* pAttr = pRequest->get(nIndex);
        switch (pAttr->getType())
        {
#if 0
            case TLV_ATTR_SERVICE_GROUP_QUERY:
                {
                    handleServiceGroupQuery(pAttr, pResponse);
                }
                break;
            case TLV_ATTR_USER_CONNECTING:
                {
                    handleUserConnecting(pReqItem, pAttr);
                }
                break;
#endif
            case TLV_ATTR_USER_AUTH:
                {
                    handleUserAuth(pReqItem, pAttr);
                }
                break;
            case TLV_ATTR_CHAT:
                {
                    handleChatInfo(pReqItem, pAttr, &pResponse);
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

    if (pResponse.size() > 0)
    {
        // build messages with header & tail info
        TempSimpleVector_c<TlvAttrIf_i*> pResp(8);
        handleVersionAttr(&pResp);

        for (int nIndex=0; nIndex<pResponse.size(); nIndex++)
        {
            pResp.append(pResponse.get(nIndex));
        }

        handleTailAttr(&pResp);
        pResponse.clear();

        TempMemBuffer_c tbResponse;
        TlvEncoder_c tlvEncoder;
        tlvEncoder.generate(&pResp, tbResponse);

        IO_SOCKET_WRITE(pReqItem->getSocketFd(), tbResponse);

        // release message items
        for (int nIndex=pResp.size()-1; nIndex>=0; nIndex--)
        {
            delete pResp.takeLast();
        }
    }

#if 0
    // append tail 
    handleTailAttr(pResponse);
#endif

    return true;
}

bool handleVersionAttr(TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    TlvAttrIf_i* tlvVer = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
    if (NULL == tlvVer)
    {
        return false;
    }

    // TODO: get version from system info
    //  or: retrive the client version, and the following message handling may depend on it
    tlvVer->setValue_int32(0x0001);
    pResponse->append(tlvVer);

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

#if 0
bool handleUserConnecting(RequestItemIf_i* pReqItem, TlvAttrIf_i* pAttr)
{
    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }

    return true;
}
#endif

// TODO: to be reorganized: move to UserInfo_c
bool sendAuthResponse(UserInfo_c* pUser, long long nUserId)
{
    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    // send response to client
    TlvAttrIf_i* pTlvUser = createTlvAttribute(TLV_ATTR_USER_INFO);
    if (NULL == pTlvUser)
    {
        return false;
    }

    // userid
    TlvAttrIf_i* pTlvUserId = createTlvAttribute(TLV_ATTR_USER_ID);
    if (NULL == pTlvUserId)
    {
        delete pTlvUser;
        return false;
    }
    pTlvUserId->setValue_int64(nUserId);
    pTlvUser->appendAttr(pTlvUserId);

    // server time
    TlvAttrIf_i* pTlvDate = createTlvAttribute(TLV_ATTR_DATE);
    if (NULL == pTlvDate)
    {
        delete pTlvUser;
        return false;
    }
    pTlvDate->setValue_int64(Time_c::getTimestamp());
    pTlvUser->appendAttr(pTlvDate);

    lstAttrs.append(pTlvUser);

    pUser->sendMessage(&lstAttrs);

    for (int nAttrInd=lstAttrs.size()-1; nAttrInd>=0; nAttrInd--)
    {
        delete lstAttrs.takeLast();
    }

    return true;
}

bool handleUserAuth(RequestItemIf_i* pReqItem, TlvAttrIf_i* pAttr)
{
    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }
    
    long long nUserId   = 0;
    const char* sPassword = NULL;
    int nPwdLength      = 0;
    int nVerifyKey      = 0;

    // 1. get user auth related info
    {
        TlvAttrIf_i* pItem  = NULL;
        for (;(pItem = pIterator->next());)
        {
            switch (pItem->getType())
            {
                case TLV_ATTR_USER_ID:
                    nUserId = pItem->getInt64();
                    break;
                case TLV_ATTR_USER_PASSWORD:
                    sPassword   = pItem->getValue();
                    nPwdLength  = pItem->getValueLen();
                    break;
                case TLV_ATTR_USER_VERIFY_KEY:
                    nVerifyKey  = pItem->getInt32();
                default:;
            }
        }
    }

    delete pIterator;

    // 2. verify user 
    if ((0 == nUserId) || (NULL == sPassword) || (0 >= nPwdLength))
    {
        return false;
    }

    UserInfo_c* pUser = UserCenter_c::getInstance()->lookupUser(nUserId);
    if (NULL == pUser)
    {
        MSG("Could not find user [%d] at service group\n", nUserId);

        //TODO: try to find user's serving group, and send redirect back
        return false;
    }

    // TODO: verify user password info & verify key
    {
    }

    // ... passed, bind requester and handler
    pReqItem->setHandler(pUser);
    pUser->setRequester(pReqItem);

    // send response message
    sendAuthResponse(pUser, nUserId);

    return true;
}

// handle chat request from other service group, it must be destination of this chat
bool handleChatInfo(RequestItemIf_i* pReqItem, TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }

    // organize chat info
    long long nUserId   = 0;
    long long nFrom     = 0;
    long long nTo       = 0;
    int nChatId         = 0;

    TlvAttrIf_i* pItem = NULL;
    for (;(pItem = pIterator->next());)
    {
        switch (pItem->getType())
        {
            case TLV_ATTR_USER_ID:
                {
                    nUserId = pItem->getInt64();
                    break;
                }
            case TLV_ATTR_CHAT_ID:
                {
                    nChatId = pItem->getInt32();
                    break;
                }
            case TLV_ATTR_CHAT_FROM:
                {
                    nFrom = pItem->getInt64();
                    break;
                }
            case TLV_ATTR_CHAT_TO:
                {
                    nTo = pItem->getInt64();
                    break;
                }
            default:;
        }
    }
    delete pIterator;

    // 
    if ((0 == nUserId) || (0 == nFrom) || (0 == nTo))
    {
        MSG("illegal chat info\n");
        return false;
    }

    if (nFrom != nUserId)
    {
        MSG_ERR("It's not chat destination, error encountered\n");
        return false;
    }

    // find user based on TO
    UserInfo_c* pUser = UserCenter_c::getInstance()->lookupUser(nTo);
    if (NULL == pUser)
    {
        MSG_ERR("Could not find user [%d]\n", nTo);
        return false;
    }

    if (pUser->handleChatInfo(pAttr))
    {
        // build chat reponse to source service group
        TlvAttrIf_i* pTlvResp = createTlvAttribute(TLV_ATTR_CHAT_RESP);
        if (NULL == pTlvResp)
        {
            return false;
        }

        // user id
        TlvAttrIf_i* pTlvUserId = createTlvAttribute(TLV_ATTR_USER_ID);
        if (NULL == pTlvUserId)
        {
            delete pTlvResp;
            return false;
        }
        pTlvUserId->setValue_int64(nUserId);
        pTlvResp->appendAttr(pTlvUserId);

        // chat id
        TlvAttrIf_i* pTlvChatId = createTlvAttribute(TLV_ATTR_CHAT_ID);
        if (NULL == pTlvChatId)
        {
            delete pTlvResp;
            return false;
        }
        pTlvChatId->setValue_int32(nChatId);
        pTlvResp->appendAttr(pTlvChatId);

        pResponse->append(pTlvResp);
    }
    else
    {
        // TODO: build failure message to source SG
    }

    return true;
}

#if 0
bool handleServiceGroupQuery(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    TlvAttrIf_i* tlvSg = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_UPDATE);
    if (NULL == tlvSg)
    {
        return false;
    }

    TlvAttrIterator_i* pIterator = tlvSg->getIterator();
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
#endif

