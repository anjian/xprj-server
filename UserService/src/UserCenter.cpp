
#include <common/inc/MyDebug.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>
#include <interface/protocol/TlvValueDef.h>

#include <UserService/inc/ChatInfo.h>
#include <UserService/inc/UserInfo.h>
#include <UserService/inc/UserCenter.h>

UserCenter_c::UserCenter_c() :
    htUserInfo_m()
{
}

UserCenter_c::~UserCenter_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(UserCenter_c)

bool UserCenter_c::insertUser(UserInfo_c* pUser)
{
    if (NULL == pUser)
    {
        return false;
    }

    UserInfo_c* pOrigUser = htUserInfo_m.lookup(pUser->getUserId());
    if (NULL != pOrigUser)
    {
        MSG_ERR("The user (%X) has been existed!\n", pUser->getUserId());
        return false;
    }

    htUserInfo_m.insert(pUser->getUserIdAddress(), pUser);

    MSG("Insert user:\n");
    MSG("   ID[%d]\n", pUser->getUserId());
    MSG("   SName[%s]\n", pUser->getScreenName());
    MSG("   level[%d]\n", pUser->getUserLevel());

    return true;
}

UserInfo_c* UserCenter_c::lookupUser(long long nId)
{
    return htUserInfo_m.lookup(nId);
}

void UserCenter_c::removeUser(long long nId)
{
    UserInfo_c* pUser = htUserInfo_m.remove(nId);
    if (NULL != pUser)
    {
        delete pUser;
    }
}

#if 0
bool UserCenter_c::handleChatInfo(TlvAttrIf_i* pAttr)
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
    long long nUserId   = -1;
    long long nFrom     = -1;
    long long nTo       = -1;
    long long nDate     = -1;
    int nType           = TLV_CHAT_TYPE_TEXT;
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
    if ((-1 == nFrom) || (-1 == nTo) || (NULL == pContent) || (0 >= nContentLen))
    {
        MSG("illegal chat info\n");
        return false;
    }

    UserInfo_c* pUser = lookupUser(nUserId);
    if (NULL == pUser)
    {
        MSG("Could not find user [%d]\n", nUserId);
        return false;
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

    pUser->addChat(pChat, pAttr);
    
    return true;
}
#endif

bool UserCenter_c::sendChatTo(long long nUserId, TlvAttrIf_i* pTlvChat)
{
    UserInfo_c* pUser = lookupUser(nUserId);
    if (NULL == pUser)
    {
        MSG("Could not find user [%d]\n", nUserId);
        return false;
    }

    pUser->receiveChat(pTlvChat);
    return true;
}
