
#include <common/inc/MyDebug.h>
#include <common/inc/os_type.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/system/SystemInfoIf.h>

#include <interface/service_group/ServiceGroupIf.h>

#include <UserService/inc/UserServiceInit.h>

////////////////////////////////////////////////////////////////////////////////
// test code, to be removed
////////////////////////////////////////////////////////////////////////////////
//
#include <interface/baseos/TempMemBuffer.h>

#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>

bool tlvTest()
{
    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    ////////////////////////////////////////////////////////////////////////////////
    // encode
    ////////////////////////////////////////////////////////////////////////////////
    // append protocol version
    TlvAttrIf_i* tlvTest = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
    tlvTest->setValue_int32(0x00010000);

    lstAttrs.append(tlvTest);

    // set attribute test
    {
        TlvAttrIf_i* tlvUserInfo = createTlvAttribute(TLV_ATTR_USER_INFO);

        // user id
        TlvAttrIf_i* tlvUserId = createTlvAttribute(TLV_ATTR_USER_ID);
        tlvUserId->setValue_int64(0x0102030405060708L);
        tlvUserInfo->appendAttr(tlvUserId);
        
        // password
        TlvAttrIf_i* tlvUserPwd = createTlvAttribute(TLV_ATTR_USER_PASSWORD);
        tlvUserPwd->setValue_buffer("test", 4);
        tlvUserInfo->appendAttr(tlvUserPwd);

        lstAttrs.append(tlvUserInfo);
    }

    // append tail flag
    TlvAttrIf_i* tlvTail = createTlvAttribute(TLV_ATTR_TAIL_FLAG);
    tlvTail->setValue_int32(0);

    lstAttrs.append(tlvTail);

    TempMemBuffer_c tbOutput;
    TlvEncoder_c tlvEncoder;
    tlvEncoder.generate(&lstAttrs, tbOutput);

    for (int nIndex=lstAttrs.size()-1; nIndex>=0; nIndex--)
    {
        delete lstAttrs.takeLast();
    }


    ////////////////////////////////////////////////////////////////////////////////
    // decode
    ////////////////////////////////////////////////////////////////////////////////
    //
    TempSimpleVector_c<TlvAttrIf_i*> lstDecodedAttrs(8);

    TlvParser_c tlvParser(&lstDecodedAttrs);
    const char* pBuffer = tbOutput.getBuffer();
    int nLeftLen        = tbOutput.getLength();

    int nStep           = 1;

    for (; nLeftLen > 0; nLeftLen -= nStep)
    {
        int nDecode = (nLeftLen > nStep) ? nStep : nLeftLen;
        if (TLV_PARSE_DONE == tlvParser.parse(pBuffer, nDecode))
        {
            MSG("decoded attributes: %d\n", lstDecodedAttrs.size());
        }

        pBuffer += nStep;
    }

    return true;
}


#include <stdio.h>
#include <sqlite3.h>

int sqliteTest(void)
{
    sqlite3 *conn;
    sqlite3_stmt    *res;
    int error = 0;
    int rec_count = 0;
    //const char *errMSG;
    const char *tail;

    //#define DATABASE ":memory:"
    error = sqlite3_open("ljdata.sl3", &conn);
    if (error)
    {
        puts("Can not open database");
        return 1;
    }

    error = sqlite3_exec(conn,
            "update people set phonenumber=\'5055559999\' where id=3",
            0, 0, 0);

    error = sqlite3_prepare_v2(conn,
            "select lastname,firstname,phonenumber,id from people order by id",
            1000, &res, &tail);

    //sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "END TRANSACTION", NULL, NULL, &sErrMsg);
    //
    //sqlite3_exec(db, "PRAGMA synchronous = OFF", NULL, NULL, &sErrMsg);
    //sqlite3_exec(db, "PRAGMA journal_mode = MEMORY", NULL, NULL, &sErrMsg);
    //  PRAGMA foreign_keys = ON

    if (error != SQLITE_OK)
    {
        sqlite3_close(conn);
        puts("We did not get any data!");
        return 1;
    }

    puts("==========================");

    while (sqlite3_step(res) == SQLITE_ROW)
    {
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%u\n", sqlite3_column_int(res, 3));

        rec_count++;
    }

    puts("==========================");
    printf("We received %d records.\n", rec_count);

    sqlite3_finalize(res);

    sqlite3_close(conn);

    return 0;
}

#include <UserService/inc/UserInfo.h>
#include <UserService/inc/UserCenter.h>

#if 0
int userTest()
{
    UserCenter_c* pUserCenter = UserCenter_c::getInstance();

    // 1st user
    UserInfo_c* pUser = new UserInfo_c;
    pUser->setUserId(0x1000L);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james");
    pUser->setLevel(1);

#if 0
    pUser->addBuddy(0x1007L, 1);
    pUser->addBuddy(0x100EL, 2);
    pUser->addBuddy(0x1015L, 3);
    pUser->addBuddy(0x101cL, 4);
    pUser->addBuddy(0x101dL, 2);

    pUser->removeBuddy(0x1015L);

    pUser->debug();
#endif

    pUserCenter->insertUser(pUser);

    // 2nd user
    pUser = new UserInfo_c;
    pUser->setUserId(0x1007L);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james2");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);

    // 3rd one
    pUser = new UserInfo_c;
    pUser->setUserId(0x100EL);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james3");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);

    // 4th one
    pUser = new UserInfo_c;
    pUser->setUserId(0x100FL);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james4");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);

    // 5th one
    pUser = new UserInfo_c;
    pUser->setUserId(0x1010L);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james5");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);

    // 5th one
    pUser = new UserInfo_c;
    pUser->setUserId(0x1015L);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james6");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);

    // 6th one
    pUser = new UserInfo_c;
    pUser->setUserId(0x101cL);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james7");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);

    // 7th one
    pUser = new UserInfo_c;
    pUser->setUserId(0x101dL);
    pUser->setServiceGroupId(1);
    pUser->setScreenName("james8");
    pUser->setLevel(1);

    pUserCenter->insertUser(pUser);


    UserInfo_c* pFindUser = pUserCenter->lookupUser(0x1007L);
    if (NULL == pFindUser)
    {
        return -1;
    }
    else
    {
        MSG("User found!\n");
    }
    
    pUserCenter->removeUser(0x1007L);

    pFindUser = pUserCenter->lookupUser(0x1007L);
    if (NULL == pFindUser)
    {
        MSG("User not found!\n");
        return -1;
    }
    else
    {
        MSG("User found!\n");
    }

    return 0;
}
#endif

#if 0
#include <UserService/inc/ServiceGroupInfo.h>
int sgTest()
{
    ServiceGroupMgr_c::getInstance()->addServiceGroup(0x01);

    return 0;
}
#endif
//
////////////////////////////////////////////////////////////////////////////////
// test code, to be removed
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// User Service init
////////////////////////////////////////////////////////////////////////////////
//
UserServiceInit_c::UserServiceInit_c()
{
}

UserServiceInit_c::~UserServiceInit_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(UserServiceInit_c)

bool UserServiceInit_c::initialize()
{
    // set user service's SG Manager as slave: to sync SG info from Dispatcher
    SERVICE_GROUP_INFO_SET_SLAVE();

    //userTest();

#if 0
    tlvTest();
    sqliteTest();

    sgTest();
#endif
    if (!loadUserData())
    {
        return false;
    }

    return true;
}

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
#if 0
        printf("%s|", sqlite3_column_text(res, 0));
        printf("%s|", sqlite3_column_text(res, 1));
        printf("%s|", sqlite3_column_text(res, 2));
        printf("%u\n", sqlite3_column_int(res, 3));

        //rec_count++;
#endif
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
