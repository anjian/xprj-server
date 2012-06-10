#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>

//#include <interface/system/GlobalInstanceMgr.h>

#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>
#include <interface/listener/RequestHandlerIf.h>
#include <interface/listener/ListenerIf.h>
#include <interface/service_group/ServiceGroupIf.h>

#include <dispatcher/inc/UserInfoMgr.h>
#include <dispatcher/inc/UserAccountMgr.h>
#include <dispatcher/inc/RequestHandler.h>

////////////////////////////////////////////////////////////////////////////////
// declare private functions
////////////////////////////////////////////////////////////////////////////////
bool handleVersionAttr(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
bool handleTailAttr(TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
bool handleServiceGroupQuery(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);
bool handleUserConnecting(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse);

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
            case TLV_ATTR_USER_CONNECTING:
                {
                    handleUserConnecting(pAttr, &pResponse);
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
    // 0. analyze request
    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
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
            return false;
        }
    }

    MSG("Check Service Group Info: %d\n", nSgId);

    // get service group info
    const char* sHost = NULL;
    const char* sPort = NULL;

    if (!SERVICE_GROUP_INFO_QUERY(nSgId, sHost, sPort) || (NULL == sHost) || (NULL == sPort))
    {
        return false;
    }

    TlvAttrIf_i* tlvSg = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_UPDATE);
    if (NULL == tlvSg)
    {
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

bool handleUserConnecting(TlvAttrIf_i* pAttr, TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    // 0. get user ID/TYPE
    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }

    const char* sAccount = NULL;
    int nType = 0;

    {
        TlvAttrIf_i* pItem = NULL;
        for (;(pItem = pIterator->next());)
        {
            switch (pItem->getType())
            {
                case TLV_ATTR_USER_ACCOUNT:
                    sAccount = pItem->getString();
                    break;
                case TLV_ATTR_USER_ACCOUNT_TYPE:
                    nType = pItem->getInt32();
                    break;
                default:;
            }
        }
        delete pIterator;
    }

    if (NULL == sAccount)
    {
        return false;
    }

    MSG("Check User Info: %s\n", sAccount);

    long long nUserId = -1;
    int nSgId = 0;
    const char* sHost = NULL;
    const char* sPort = NULL;
    //int nIpType = 0;                // IPv4

    // 1 get information related to this account
    //
    // 1.1 get user id
    UserAccountMgr_c* pAccountMgr = UserAccountMgr_c::getInstance();
    if (!pAccountMgr->query(sAccount, nType, nUserId) || (0 >= nUserId))
    {
        // TODO: reponse account not found
        return false;
    }

    // 1.2 get service group id
    UserInfoMgr_c* pUserInfo = UserInfoMgr_c::getInstance();
    if ((nSgId = pUserInfo->getSgId(nUserId)) <= 0)
    {
        // TODO: ridiculous, could not find user service group id
        return false;
    }

    // 1.3 get service group info
    //ServiceGroupMgr_c* pSgMgr = ServiceGroupMgr_c::getInstance();
    if (!SERVICE_GROUP_INFO_QUERY(nSgId, sHost, sPort) || (NULL == sHost) || (NULL == sPort))
    {
        // TODO: system internal error: could not get service group info
        return false;
    }


    // 2. build reponse

    // 2.1 user info
    TlvAttrIf_i* tlvUserInfo = createTlvAttribute(TLV_ATTR_USER_INFO);
    if (NULL == tlvUserInfo)
    {
        return false;
    }

    // 2.1.1 User Id
    {
        TlvAttrIf_i* tlvUserId = createTlvAttribute(TLV_ATTR_USER_ID);
        if (NULL == tlvUserId)
        {
            delete tlvUserInfo;
            return false;
        }

        tlvUserId->setValue_int64(nUserId);
        tlvUserInfo->appendAttr(tlvUserId);
    }

    // 2.1.2 verify key
    {
        TlvAttrIf_i* tlvVerifyKey = createTlvAttribute(TLV_ATTR_USER_VERIFY_KEY);
        if (NULL == tlvVerifyKey)
        {
            delete tlvUserInfo;
            return false;
        }

        // TODO: get veirfy key
        tlvVerifyKey->setValue_int32(0);
        tlvUserInfo->appendAttr(tlvVerifyKey);
    }

    // 2.2 sg info
    TlvAttrIf_i* tlvSg = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_UPDATE);
    if (NULL == tlvSg)
    {
        delete tlvUserInfo;
        return false;
    }

    // 2.2.1 Sg Id
    {
        TlvAttrIf_i* tlvSgId = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_ID);
        if (NULL == tlvSgId)
        {
            delete tlvUserInfo;
            delete tlvSg;

            return false;
        }

        tlvSgId->setValue_int32(nSgId);
        tlvSg->appendAttr(tlvSgId);
    }

    // 2.2.2 sg host 
    {
        TlvAttrIf_i* tlvSgHost = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_HOST);
        if (NULL == tlvSgHost)
        {
            delete tlvUserInfo;
            delete tlvSg;

            return false;
        }

        tlvSgHost->setValue_buffer(sHost, xstrlen(sHost));
        tlvSg->appendAttr(tlvSgHost);
    }

    // 2.2.3 sg port
    {
        TlvAttrIf_i* tlvSgPort = createTlvAttribute(TLV_ATTR_SERVICE_GROUP_PORT);
        if (NULL == tlvSgPort)
        {
            delete tlvSg;
            return false;
        }

        tlvSgPort->setValue_int32(xatoi(sPort));
        tlvSg->appendAttr(tlvSgPort);
    }

    // append items into reponse list
    pResponse->append(tlvUserInfo);
    pResponse->append(tlvSg);

    return true;
}
