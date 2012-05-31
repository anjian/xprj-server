#ifndef __USER_INFO_MGR_H_
#define __USER_INFO_MGR_H_

#include <common/inc/MyHash.h>

// TODO: to user page to keep user info

struct UserSgInfo_t
{
    UserSgInfo_t() :
        nId_m(0),
        nSgId_m(0)
    {
    }

    inline long long* getUserIdAddress()    { return &nId_m; }

    inline long long getUserId()            { return nId_m; }
    inline int getSgId()                    { return nSgId_m; }

    inline void setUserId(long long nId)    { nId_m = nId; }
    inline void setSgId(int nId)            { nSgId_m = nId; }

    long long nId_m;
    int nSgId_m;
};

class UserInfoMgr_c
{
    public:
        ~UserInfoMgr_c();
        static UserInfoMgr_c* getInstance();

        bool insert(long long nUserId, int nSgId);
        int getSgId(long long nUserId);

        bool remove(long long nUserId);
    protected:
        UserInfoMgr_c();

    private:
        HashLong_c<UserSgInfo_t*> htUserSgInfo_m;
};

#endif
