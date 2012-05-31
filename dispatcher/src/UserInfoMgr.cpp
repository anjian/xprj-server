#include <common/inc/MyDebug.h>

#include <interface/system/GlobalInstanceMgr.h>

#include <dispatcher/inc/UserInfoMgr.h>

UserInfoMgr_c::UserInfoMgr_c()
{
}

UserInfoMgr_c::~UserInfoMgr_c()
{
    //TODO: release memory after exit, need to handle properly
    //htUserSgInfo_m.remove_all();
}

IMPLE_OBJECT_GET_INSTANCE(UserInfoMgr_c)

bool UserInfoMgr_c::insert(long long nUserId, int nSgId)
{
    UserSgInfo_t* pUser = htUserSgInfo_m.lookup(nUserId);
    if (NULL != pUser)
    {
        MSG_ERR("The user (%X) has been existed!\n", nUserId);
        return false;
    }

    pUser = new UserSgInfo_t;
    if (NULL == pUser)
    {
        MSG_ERR("Could not allocate memory for user sg info (%x)\n", nUserId);
        return false;
    }

    pUser->setUserId(nUserId);
    pUser->setSgId(nSgId);

    htUserSgInfo_m.insert(pUser->getUserIdAddress(), pUser);

    return true;
}

int UserInfoMgr_c::getSgId(long long nUserId)
{
    UserSgInfo_t* pUser = htUserSgInfo_m.lookup(nUserId);
    if (NULL == pUser)
    {
        MSG("Could not find user (%X)\n", nUserId);
        return 0;
    }

    return pUser->getSgId();
}

bool UserInfoMgr_c::remove(long long nUserId)
{
    UserSgInfo_t* pUser = htUserSgInfo_m.lookup(nUserId);
    if (NULL != pUser)
    {
        delete pUser;
    }

    return true;
}
