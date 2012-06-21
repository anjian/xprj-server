#ifndef __USER_CENTER_H_
#define __USER_CENTER_H_

#include <common/inc/MyHash.h>

class UserInfo_c;
class TlvAttrIf_i;
class ChatInfo_c;

class UserCenter_c
{
    public:
        ~UserCenter_c();

        static UserCenter_c* getInstance();

        bool insertUser(UserInfo_c* pUser);
        UserInfo_c* lookupUser(long long nId);
        void removeUser(long long nId);

        //bool handleChatInfo(TlvAttrIf_i* pAttr);
        bool sendChatTo(long long nUserId, TlvAttrIf_i* pTlvChat);

    protected:
        UserCenter_c();

    private:
        // All users managered by this group
        HashLong_c<UserInfo_c*> htUserInfo_m;

        // All buddies of users'
        HashLong_c<UserInfo_c*> htBuddiesInfo_m;
};

#endif
