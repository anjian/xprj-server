
#include <common/inc/MyStdLibrary.h>
#include <common/inc/MyTime.h>
#include <common/inc/MyDebug.h>

#include <interface/baseos/MemoryIf.h>
#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>
#include <interface/data_proxy/DataProxyIf.h>

#include <UserService/inc/ChatInfo.h>
#include <UserService/inc/UserCenter.h>
#include <UserService/inc/UserInfo.h>

#define ONLINE_THRESHOLD       120 

UserInfo_c::UserInfo_c() :
    nUserId_m(0),
    nServiceGroupId_m(0),
    sScreenName_m(NULL),
    nLevel_m(0),
    nLastHeartBeat_m(0),
    pPixBuffer_m(NULL),
    nPixLen_m(0)
{
}

UserInfo_c::~UserInfo_c()
{
    if (NULL != sScreenName_m)
    {
        xfree(sScreenName_m);
    }

    if (NULL != pPixBuffer_m)
    {
        xfree(pPixBuffer_m);
    }
}

void UserInfo_c::setScreenName(const char* sName)
{
    if (NULL == sName)
    {
        return;
    }

    if (NULL != sScreenName_m)
    {
        xfree(sScreenName_m);
    }

    int nNameLen = xstrlen(sName);
    sScreenName_m = (char*)xmalloc(nNameLen + 1);
    if (NULL == sScreenName_m)
    {
        return;
    }

    xmemcpy(sScreenName_m, sName, nNameLen);
    sScreenName_m[nNameLen] = 0;
}

void UserInfo_c::setUserPixmap(const void* pBuf, int nLen)
{
    if ((NULL == pBuf) || (nLen <= 0))
    {
        return;
    }

    if (NULL != pBuf)
    {
        xfree(pPixBuffer_m);
    }

    pPixBuffer_m = (char*)xmalloc(nLen);
    if (NULL == pPixBuffer_m)
    {
        return;
    }

    xmemcpy(pPixBuffer_m, pBuf, nLen);
    nPixLen_m = nLen;
}

bool UserInfo_c::isOnline()
{
    return ((Time_c::getTimestamp() - nLastHeartBeat_m) < ONLINE_THRESHOLD);
}

void UserInfo_c::addBuddy(long long nBuddyId, int nSGId)
{
    BuddyStatus_t tBuddy;

    tBuddy.nBuddyId_m = nBuddyId;
    tBuddy.nServiceGroupId_m = nSGId;

    lstBuddys_m.append(tBuddy);

    MSG("Add [%s]'s buddy: %d\n", getScreenName(), nBuddyId);
}

void UserInfo_c::removeBuddy(long long nBuddyId)
{
    for (int nIndex=lstBuddys_m.size()-1; nIndex>=0; nIndex--)
    {
        BuddyStatus_t* pBuddy = lstBuddys_m.getRef(nIndex);
        if ((NULL != pBuddy) && (pBuddy->nBuddyId_m == nBuddyId))
        {
            lstBuddys_m.takeAt(nIndex);
            return;
        }
    }

    return;
}

void UserInfo_c::processRequest(TempSimpleVector_c<TlvAttrIf_i*>* lstRequest)
{
    // handle content
    for (int nIndex=1; nIndex<lstRequest->size(); nIndex++)
    {
        TlvAttrIf_i* pAttr = lstRequest->get(nIndex);
        switch (pAttr->getType())
        {
            case TLV_ATTR_CHAT:
                {
                    handleChatInfo(pAttr);
                }
                break;
            case TLV_ATTR_USER_LOGOUT:
                {
                    handleLogout(pAttr);
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
}

bool UserInfo_c::handleChatInfo(TlvAttrIf_i* pAttr)
{
    if ((NULL == pAttr) || (TLV_ATTR_CHAT != pAttr->getType()))
    {
        return false;
    }

    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }

    // organize chat info
    long long nUserId   = 0;
    long long nFrom     = 0;
    long long nTo       = 0;
    long long nDate     = 0;
    int nType           = TLV_CHAT_TYPE_TEXT;
    int nChatId         = 0;
    const char* pContent= NULL;
    int nContentLen     = 0;

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
            case TLV_ATTR_DATE:
                {
                    nDate = pItem->getInt64();
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
            case TLV_ATTR_CHAT_TYPE:
                {
                    nType = pItem->getByte();
                    break;
                }
            case TLV_ATTR_CHAT_CONTENT:
                {
                    pContent = pItem->getValue();
                    nContentLen = pItem->getValueLen();
                    break;
                }
            default:;
        }
    }
    delete pIterator;

    // find service group id
    if ((0 == nFrom) || (0 == nTo) || (NULL == pContent) || (0 >= nContentLen))
    {
        MSG("illegal chat info\n");
        return false;
    }

    if (nFrom == nUserId_m)
    {
        sendChatRespMessage(nChatId);
    }

    ChatInfo_c* pChat = new ChatInfo_c;
    if (NULL == pChat)
    {
        MSG_ERR("Memory error: could not allocate memory for chat item\n");
        return false;
    }

    pChat->setFrom(nFrom);
    pChat->setTo(nTo);
    pChat->setType((TlvAttrChatType_e)nType);
    pChat->appendChat(pContent, nContentLen);

    if (-1 != nDate)
    {
        pChat->setGotTime(nDate);
    }

    addChat(pChat, pAttr);
    
    return true;
}

bool UserInfo_c::handleLogout(TlvAttrIf_i* pAttr)
{
    if ((NULL == pAttr) || (TLV_ATTR_USER_LOGOUT != pAttr->getType()))
    {
        return false;
    }

    TlvAttrIterator_i* pIterator = pAttr->getIterator();
    if (NULL == pIterator)
    {
        return false;
    }

    // organize chat info
    long long nUserId   = 0;
    int nVerifyKey      = 0;

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
            case TLV_ATTR_USER_VERIFY_KEY:
                {
                    nVerifyKey = pItem->getInt32();
                    break;
                }
            default:;
        }
    }
    delete pIterator;

    // find service group id
    if ((0 == nUserId) || (0 == nVerifyKey))
    {
        MSG("illegal chat info\n");
        return false;
    }

    // TODO: verify user and verify key
    {
    }

    // passed, user logout, unbind
    unbind();
    
    return true;
}

bool UserInfo_c::sendChatRespMessage(int nChatId)
{
    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    // send response to client
    TlvAttrIf_i* pTlvResp = createTlvAttribute(TLV_ATTR_CHAT_RESP);
    if (NULL == pTlvResp)
    {
        return false;
    }

    TlvAttrIf_i* pTlvChatId = createTlvAttribute(TLV_ATTR_CHAT_ID);
    if (NULL == pTlvChatId)
    {
        delete pTlvResp;
        return false;
    }

    pTlvChatId->setValue_int32(nChatId);

    pTlvResp->appendAttr(pTlvChatId);

    lstAttrs.append(pTlvResp);
    sendMessage(&lstAttrs);

    for (int nAttrInd=lstAttrs.size()-1; nAttrInd>=0; nAttrInd--)
    {
        delete lstAttrs.takeLast();
    }

    return true;
}

void UserInfo_c::sendChatToUser(TlvAttrIf_i* pTlvChat)
{
    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    lstAttrs.append(pTlvChat);
    sendMessage(&lstAttrs);
}

void UserInfo_c::addChat(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat)
{
    if ((NULL == pChat) || (NULL == pTlvChat))
    {
        return;
    }

    if ((isOnline()) && (pChat->getTo() == nUserId_m))
    {
        sendChatToUser(pTlvChat);
    }
    else
    {
        if (pChat->getFrom() == nUserId_m)
        {
            nLastHeartBeat_m = Time_c::getTimestamp();

            // send it to destination
            sendChatToBuddy(pChat, pTlvChat);
        }
    }

    // TODO: save it into databse
    //
    return;
}

void UserInfo_c::receiveChat(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat)
{
    if ((NULL == pChat) || (pChat->getTo() != nUserId_m))
    {
        return;
    }

    if (isOnline())
    {
        sendChatToUser(pTlvChat);
        //lstChats_m.append(pChat);
    }

    // TODO: save it into databse
}

void UserInfo_c::sendChatToBuddy(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat)
{
    int nBuddySGId = getBuddySGId(pChat->getTo());

    if (0 >= nBuddySGId)
    {
        // TODO: send message to a stranger
        return;
    }

    if (nBuddySGId != nServiceGroupId_m)
    {
        // send by network message
        sendChatByNet(nBuddySGId, pTlvChat);
    }
    else
    {
        // the buddy at the same service group, route a duplicate message directly
        ChatInfo_c* pNewChat = new ChatInfo_c(pChat);
        if (NULL != pNewChat)
        {
            UserCenter_c::getInstance()->sendChatTo(pNewChat, pTlvChat);
        }
    }
}

int UserInfo_c::getBuddySGId(long long nBuddyId)
{
    for (int nIndex=0; nIndex<lstBuddys_m.size(); nIndex++)
    {
        if (lstBuddys_m.getRef(nIndex)->nBuddyId_m == nBuddyId)
        {
            return lstBuddys_m.getRef(nIndex)->nServiceGroupId_m;
        }
    }

    return -1;
}

bool UserInfo_c::sendChatByNet(int nSgId, TlvAttrIf_i* pTlvChat)
{
    // encode message content
    TempMemBuffer_c tbBuffer;
    TlvEncoder_c tlvEncoder;
    tlvEncoder.generate(pTlvChat, tbBuffer);

    MSG("insert app event\n");
    SEND_NET_REQUEST(nSgId, tbBuffer.getBuffer(), tbBuffer.getLength());

    return true;
}

void UserInfo_c::debug()
{
    MSG("User [%X] Buddies:\n", getUserId());
    for (int nIndex=lstBuddys_m.size()-1; nIndex>=0; nIndex--)
    {
        BuddyStatus_t* pBuddy = lstBuddys_m.getRef(nIndex);
        if (NULL != pBuddy)
        {
            MSG("   id [%X]\n", pBuddy->nBuddyId_m);
        }
    }
}
