#include <sqlite3.h>

#include <common/inc/MyDebug.h>
#include <common/inc/os_type.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/system/SystemInfoIf.h>

#include <interface/service_group/ServiceGroupIf.h>

#include <dispatcher/inc/UserInfoMgr.h>
#include <dispatcher/inc/UserAccountMgr.h>
#include <dispatcher/inc/dispatcherInit.h>

////////////////////////////////////////////////////////////////////////////////
// dispatcher service init
////////////////////////////////////////////////////////////////////////////////
//
DispatcherInit_c::DispatcherInit_c()
{
}

DispatcherInit_c::~DispatcherInit_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(DispatcherInit_c)

bool DispatcherInit_c::initialize()
{
    if (!loadDataFromDB())
    {
        MSG_ERR("Could not load data from DB\n");
        return false;
    }

    SERVICE_GROUP_INFO_LOAD();

    return true;
}

bool DispatcherInit_c::loadDataFromDB()
{
    const char* sDB = SYS_INFO_GET_STRING(SYS_INFO_TYPE_SERVICE_DATABASE);
    if (NULL == sDB)
    {
        MSG_ERR("User Info DB is not configured\n");
        return false;
    }

    sqlite3 *pConn;
    if (sqlite3_open(sDB, &pConn))
    {
        MSG_ERR("Can not open database [%d]\n", sDB);
        return false;
    }

    if (!loadUserInfo(pConn))
    {
        MSG_ERR("load user id info error\n");
        sqlite3_close(pConn);

        return false;
    }

    if (!loadUserAccount(pConn))
    {
        MSG_ERR("load user account info error\n");
        sqlite3_close(pConn);

        return false;
    }

#if 0
    if (!loadSgInfo(pConn))
    {
        MSG_ERR("load service group info error\n");
        sqlite3_close(pConn);

        return false;
    }
#endif

    sqlite3_close(pConn);

    return true;
}

bool DispatcherInit_c::loadUserInfo(sqlite3 *pConn)
{
    //const char *errMSG;

    const char* sUserQuery = "select id, sgid from userinfo";
    //sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    //
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    //  PRAGMA foreign_keys = ON

    sqlite3_stmt *res;
    const char *tail;

    if (SQLITE_OK != sqlite3_prepare_v2(pConn,
                sUserQuery,
                1000,
                &res,
                &tail))
    {
        sqlite3_close(pConn);

        MSG_ERR("Could not get data from DB\n");
        return false;
    }

    UserInfoMgr_c* pUserInfo = UserInfoMgr_c::getInstance();

    //int rec_count = 0;
    while (SQLITE_ROW == sqlite3_step(res))
    {
        pUserInfo->insert(
                sqlite3_column_int64(res, 0),
                sqlite3_column_int(res, 1));
    }

    //puts("==========================");
    //printf("We received %d records.\n", rec_count);

    sqlite3_finalize(res);

    return true;
}

bool DispatcherInit_c::loadUserAccount(sqlite3 *pConn)
{
    //const char *errMSG;

    const char* sUserQuery = "select id, account, type from useraccount";
    //sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    //
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    //  PRAGMA foreign_keys = ON

    sqlite3_stmt *res;
    const char *tail;

    if (SQLITE_OK != sqlite3_prepare_v2(pConn,
                sUserQuery,
                1000,
                &res,
                &tail))
    {
        sqlite3_close(pConn);

        MSG_ERR("Could not get data from DB\n");
        return false;
    }

    UserAccountMgr_c* pUserAccount = UserAccountMgr_c::getInstance();

    //int rec_count = 0;
    while (SQLITE_ROW == sqlite3_step(res))
    {
        pUserAccount->insert(
                (const char*)sqlite3_column_text(res, 1),
                sqlite3_column_int64(res, 0),
                sqlite3_column_int(res, 2));
    }

    //puts("==========================");
    //printf("We received %d records.\n", rec_count);

    sqlite3_finalize(res);

    return true;
}

#if 0
bool DispatcherInit_c::loadSgInfo(sqlite3 *pConn)
{
    return true;
}
#endif

#if 0
bool UserServiceInit_c::loadUserData()
{
    const char* sDB = SYS_INFO_GET_STRING(SYS_INFO_TYPE_SERVICE_DATABASE);
    if (NULL == sDB)
    {
        MSG_ERR("User Info DB is not configured\n");
        return false;
    }

    sqlite3 *pConn;
    if (sqlite3_open(sDB, &pConn))
    {
        MSG_ERR("Can not open database [%d]\n", sDB);
        return false;
    }

    if (!loadUserInfo(pConn))
    {
        MSG_ERR("load user info error\n");
        sqlite3_close(pConn);

        return false;
    }

    if (!loadUserBuddies(pConn))
    {
        MSG_ERR("load user buddy error\n");
        sqlite3_close(pConn);

        return false;
    }

    sqlite3_close(pConn);

    return true;
}

bool UserServiceInit_c::loadUserInfo(sqlite3 *pConn)
{
    //const char *errMSG;

    const char* sUserQuery = "select id, screenname, pic, level from userinfo";
    //sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    //
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    //  PRAGMA foreign_keys = ON

    sqlite3_stmt *res;
    const char *tail;

    if (SQLITE_OK != sqlite3_prepare_v2(pConn,
                sUserQuery,
                1000,
                &res,
                &tail))
    {
        sqlite3_close(pConn);

        MSG_ERR("Could not get data from DB\n");
        return false;
    }

    UserCenter_c* pUserCenter = UserCenter_c::getInstance();

    //int rec_count = 0;
    while (SQLITE_ROW == sqlite3_step(res))
    {
        // 1st user
        UserInfo_c* pUser = new UserInfo_c;
        if (NULL != pUser)
        {
            pUser->setUserId(sqlite3_column_int64(res, 0));
            //pUser->setServiceGroupId(sqlite3_column_int(res, 1));
            pUser->setScreenName((const char*)sqlite3_column_text(res, 1));
            pUser->setUserPixmap(sqlite3_column_blob(res, 2), sqlite3_column_bytes(res, 2));
            pUser->setLevel(sqlite3_column_int(res, 3));

            pUserCenter->insertUser(pUser);
        }
    }

    //puts("==========================");
    //printf("We received %d records.\n", rec_count);

    sqlite3_finalize(res);

    return true;
}

bool UserServiceInit_c::loadUserBuddies(sqlite3 *pConn)
{
    //const char *errMSG;
    const char* sBuddyQuery = "select buddyid, userid, buddygroupid, sgid from buddyinfo";

    sqlite3_stmt *res;
    const char *tail;

    if (SQLITE_OK != sqlite3_prepare_v2(pConn,
                sBuddyQuery,
                100,
                &res,
                &tail))
    {
        sqlite3_close(pConn);

        MSG_ERR("Could not get data from DB\n");
        return false;
    }

    UserCenter_c* pUserCenter = UserCenter_c::getInstance();

    while (SQLITE_ROW == sqlite3_step(res))
    {
        // 1st user
        UserInfo_c* pUser = pUserCenter->lookupUser(sqlite3_column_int64(res, 1));
        if (NULL != pUser)
        {
            pUser->addBuddy(sqlite3_column_int64(res, 0), sqlite3_column_int(res, 3));
        }
        else
        {
            // TODO: dirty data, to be cleaned
        }
    }

    sqlite3_finalize(res);

    return true;
}
#endif
