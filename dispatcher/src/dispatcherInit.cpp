
#include <common/inc/MyDebug.h>
#include <common/inc/os_type.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/system/SystemInfoIf.h>

#include <interface/service_group/ServiceGroupIf.h>

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
    return true;
}

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
