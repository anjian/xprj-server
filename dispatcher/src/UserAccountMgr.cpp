
#include <common/inc/MyDebug.h>

#include <interface/system/GlobalInstanceMgr.h>

#include <dispatcher/inc/UserAccountMgr.h>

UserAccountMgr_c::UserAccountMgr_c()
{
}

UserAccountMgr_c::~UserAccountMgr_c()
{
    //TODO: release memory after exit, need to handle properly
    //htUserAccount_m.remove_all();
}

IMPLE_OBJECT_GET_INSTANCE(UserAccountMgr_c)

bool UserAccountMgr_c::insert(const char* sAccount, long long nUserId, int nType)
{
    // TODO: filter space
    //
    if ((NULL == sAccount) || (0 == sAccount[0]))
    {
        MSG_ERR("inValid user accound\n");
        return false;
    }

    UserAccountInfo_t* pAccount = htUserAccount_m.lookup(sAccount);
    if (NULL != pAccount)
    {
        MSG_ERR("The account (%s) has been existed!\n", sAccount);
        return false;
    }

    pAccount = new UserAccountInfo_t;
    if (NULL == pAccount)
    {
        MSG_ERR("Could not allocate memory for account (%s)\n", sAccount);
        return false;
    }

    pAccount->setUserId(nUserId);
    pAccount->setType((UserAccountType_e)nType);

    htUserAccount_m.insert(sAccount, pAccount);

    return true;
}

bool UserAccountMgr_c::query(const char* sAccount, int nType, long long& nUserId)
{
    nUserId = 0;
    nType   = ACC_TYPE_INTERNAL;

    if ((NULL == sAccount) || (0 == sAccount[0]))
    {
        return false;
    }

    UserAccountInfo_t* pAccount = htUserAccount_m.lookup(sAccount);
    if (NULL == pAccount)
    {
        return false;
    }

    if (nType != pAccount->getType())
    {
        return false;
    }

    nUserId = pAccount->getUserId();

    return true;
}

bool UserAccountMgr_c::remove(const char* sAccount)
{
    if ((NULL == sAccount) || (0 == sAccount[0]))
    {
        return false;
    }

    UserAccountInfo_t* pAccount = htUserAccount_m.remove(sAccount);
    if (NULL != pAccount)
    {
        delete pAccount;
    }

    return true;
}
