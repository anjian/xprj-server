#ifndef __TINY_TLV_H_
#define __TINY_TLV_H_

bool isAtomicTlvAttribute(int nType);

enum TlvLengthType_e
{
    TLV_LEN_TYPE_VAR    = 0,
    TLV_LEN_TYPE_BYTE   = 1,
    TLV_LEN_TYPE_SHORT  = 2,
    TLV_LEN_TYPE_INT32  = 4,
    TLV_LEN_TYPE_INT64  = 8,
};

enum TlvAttrType_e
{
    ////////////////////////////////////////////////////////////////////////////////
    TLV_ATTR_TYPE_START = 0,
    TLV_ATTR_UNKNOWN = TLV_ATTR_TYPE_START,
    TLV_ATTR_CLIENT_VERSION,
    TLV_ATTR_TAIL_FLAG,                         // indicate the tail of message
    TLV_ATTR_COMMAND,
    TLV_ATTR_DATE,
    TLV_ATTR_SERVICE_GROUP_HOST,
    TLV_ATTR_SERVICE_GROUP_PORT,
    // !!! Please add common attribute above this line !!!
    TLV_ATTR_COMMON_END,
    //
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // service group info
    TLV_ATTR_SERVICE_GROUP_START = 501,
    TLV_ATTR_SERVICE_GROUP_QUERY = TLV_ATTR_SERVICE_GROUP_START,
    TLV_ATTR_SERVICE_GROUP_UPDATE,
    TLV_ATTR_SERVICE_GROUP_ID,

    // !!! Please add service group related attribute above this line
    TLV_ATTR_SERVICE_GROUP_END,
    //
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // user info
    TLV_ATTR_USER_TYPE_START = 1001,
    TLV_ATTR_USER_INFO = TLV_ATTR_USER_TYPE_START,
    TLV_ATTR_USER_CONNECTING,
    TLV_ATTR_USER_AUTH,
    TLV_ATTR_USER_LOGOUT,
    TLV_ATTR_USER_VERIFY_KEY,
    TLV_ATTR_USER_ID,
    TLV_ATTR_USER_ACCOUNT,
    TLV_ATTR_USER_ACCOUNT_TYPE,
    TLV_ATTR_USER_PASSWORD,
    TLV_ATTR_USER_STATUS,
    TLV_ATTR_USER_BULLETIN,
    TLV_ATTR_USER_SIGNATURE,

    TLV_ATTR_USER_PROFILE,
    TLV_ATTR_USER_SCREEN_NAME,
    TLV_ATTR_USER_FIRST_NAME,
    TLV_ATTR_USER_LAST_NAME,
    TLV_ATTR_USER_PROFILE_PIC,
    TLV_ATTR_USER_GENDER,
    TLV_ATTR_USER_BIRTHDAY,
    TLV_ATTR_USER_MOBILE,
    TLV_ATTR_USER_PHONE,
    TLV_ATTR_USER_COUNTRY,
    TLV_ATTR_USER_PROVINCE,
    TLV_ATTR_USER_CITY,
    TLV_ATTR_USER_ZIP,
    TLV_ATTR_USER_MAIL,
    TLV_ATTR_USER_JOB,
    TLV_ATTR_USER_WEBSITE,
    TLV_ATTR_USER_INTERESTED_IN,

    TLV_ATTR_USER_CREATE_DATE,
    TLV_ATTR_USER_ACTIVE_DATE,
    TLV_ATTR_USER_LEVEL,

    // !!! Please add user related attribute above this line
    TLV_ATTR_USER_END,
    //
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // chat info
    TLV_ATTR_CHAT_TYPE_START = 2001,
    TLV_ATTR_CHAT = TLV_ATTR_CHAT_TYPE_START,
    TLV_ATTR_CHAT_ID,
    TLV_ATTR_CHAT_RESP,
    TLV_ATTR_CHAT_UPDATE,
    TLV_ATTR_CHAT_FROM,
    TLV_ATTR_CHAT_TO,
    TLV_ATTR_CHAT_TYPE,
    TLV_ATTR_CHAT_CONTENT,
    TLV_ATTR_CHAT_HISTORY,                      // request from client
    TLV_ATTR_CHAT_HISTORY_DIR,                  // direction: backward (0) | forward (not 0)
    TLV_ATTR_CHAT_COUNT,

    // !!! Please add chat related attribute above this line!!!
    TLV_ATTR_CHAT_END,
    //
    ////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////
    // group info
    TLV_ATTR_GROUP_TYPE_START = 3001,
    TLV_ATTR_GROUP_INFO = TLV_ATTR_GROUP_TYPE_START,
    TLV_ATTR_GROUP_ID,
    TLV_ATTR_GROUP_SCREEN_NAME,
    TLV_ATTR_GROUP_OWNER,
    TLV_ATTR_GROUP_LEVEL,
    TLV_ATTR_GROUP_BULLETIN,

    // !!! Please add chat related attribute above this line!!!
    TLV_ATTR_GROUP_END,
    //
    ////////////////////////////////////////////////////////////////////////////////

    // please add new type above this line!!!
    TLV_ATTR_TYPE_END,
};

struct TlvAttrTypeInfo_t
{
    TlvAttrType_e eType_m;
    bool isAtomic_m;
};

#endif
