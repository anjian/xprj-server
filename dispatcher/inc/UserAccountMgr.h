#ifndef __USER_ACCOUNT_MGR_H_
#define __USER_ACCOUNT_MGR_H_

#include <common/inc/MyHash.h>

// TODO: to user page to keep user info

enum UserAccountType_e
{
    ACC_TYPE_INTERNAL,
    ACC_TYPE_FACEBOOK,

    // PLEASE add new type above this line!!!
    ACC_TYPE_END,
};

struct UserAccountInfo_t
{
    UserAccountInfo_t() :
        nUserId_m(0),
        eType_m(ACC_TYPE_INTERNAL)
    {
    }

    inline void setUserId(long long nId)            { nUserId_m = nId; }
    inline void setType(UserAccountType_e eType)    { eType_m = eType; }

    inline long long getUserId()                    { return nUserId_m; }
    inline UserAccountType_e getType()              { return eType_m; }

    // data
    long long nUserId_m;
    UserAccountType_e eType_m;
};

class UserAccountMgr_c
{
    public:
        ~UserAccountMgr_c();
        static UserAccountMgr_c* getInstance();

        bool insert(const char* sAccount, long long nUserId, int nType);
        bool query(const char* sAccount, int nType, long long& nUserId);
        bool remove(const char* sAccount);
    protected:
        UserAccountMgr_c();

    private:
        HashString_c<UserAccountInfo_t*> htUserAccount_m;
};

#endif
