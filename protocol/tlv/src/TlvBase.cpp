#include <interface/protocol/TlvIf.h>

#include <protocol/tlv/inc/TlvAttrSet.h>
#include <protocol/tlv/inc/TlvGenerator.h>

const TlvAttrTypeInfo_t tlvCommonAttrInfo_m[] = 
{
    {TLV_ATTR_UNKNOWN,                      true},
    {TLV_ATTR_CLIENT_VERSION,               true},
    {TLV_ATTR_TAIL_FLAG,                    true},
    {TLV_ATTR_COMMAND,                      true},
    {TLV_ATTR_DATE,                         true},
    {TLV_ATTR_SERVICE_GROUP_HOST,           true},
    {TLV_ATTR_SERVICE_GROUP_PORT,           true},
    {TLV_ATTR_COMMON_END,                   true},
};

const TlvAttrTypeInfo_t tlvSGAttrInfo_m[] = 
{
    {TLV_ATTR_SERVICE_GROUP_QUERY,          false},
    {TLV_ATTR_SERVICE_GROUP_UPDATE,         false},
    {TLV_ATTR_SERVICE_GROUP_ID,             true},
    {TLV_ATTR_SERVICE_GROUP_END,            true},
};

const TlvAttrTypeInfo_t tlvUserAttrInfo_m[] = 
{
    {TLV_ATTR_USER_INFO,                    false},
    {TLV_ATTR_USER_CONNECTING,              false},
    {TLV_ATTR_USER_AUTH,                    false},
    {TLV_ATTR_USER_LOGOUT,                  false},
    {TLV_ATTR_USER_VERIFY_KEY,              true},
    {TLV_ATTR_USER_ID_TYPE,                 true},
    {TLV_ATTR_USER_ID,                      true},
    {TLV_ATTR_USER_PASSWORD,                true},
    {TLV_ATTR_USER_STATUS,                  true},
    {TLV_ATTR_USER_BULLETIN,                true},
    {TLV_ATTR_USER_PROFILE,                 false},
    {TLV_ATTR_USER_SCREEN_NAME,             true},
    {TLV_ATTR_USER_PROFILE_PIC,             true},
    {TLV_ATTR_USER_GENDER,                  true},
    {TLV_ATTR_USER_BIRTHDAY,                true},
    {TLV_ATTR_USER_MOBILE,                  true},
    {TLV_ATTR_USER_PHONE,                   true},
    {TLV_ATTR_USER_COUNTRY,                 true},
    {TLV_ATTR_USER_PROVINCE,                true},
    {TLV_ATTR_USER_CITY,                    true},
    {TLV_ATTR_USER_ZIP,                     true},
    {TLV_ATTR_USER_MAIL,                    true},
    {TLV_ATTR_USER_JOB,                     true},
    {TLV_ATTR_USER_WEBSITE,                 true},
    {TLV_ATTR_USER_INTERESTED_IN,           true},
    {TLV_ATTR_USER_CREATE_DATE,             true},
    {TLV_ATTR_USER_ACTIVE_DATE,             true},
    {TLV_ATTR_USER_LEVEL,                   true},
    {TLV_ATTR_USER_END,                     true},
};

const TlvAttrTypeInfo_t tlvChatAttrInfo_m[] = 
{
    {TLV_ATTR_CHAT,                         false},
    {TLV_ATTR_CHAT_ID,                      true},
    {TLV_ATTR_CHAT_RESP,                    false},
    {TLV_ATTR_CHAT_UPDATE,                  false},
    {TLV_ATTR_CHAT_FROM,                    true},
    {TLV_ATTR_CHAT_TO,                      true},
    {TLV_ATTR_CHAT_TYPE,                    true},
    {TLV_ATTR_CHAT_CONTENT,                 true},
    {TLV_ATTR_CHAT_HISTORY,                 false},
    {TLV_ATTR_CHAT_HISTORY_DIR,             true},
    {TLV_ATTR_CHAT_COUNT,                   true},
    {TLV_ATTR_CHAT_END,                     true},
};                                                               

const TlvAttrTypeInfo_t tlvGroupAttrInfo_m[] = 
{
    {TLV_ATTR_GROUP_INFO,                   false},
    {TLV_ATTR_GROUP_ID,                     true},
    {TLV_ATTR_GROUP_SCREEN_NAME,            true},
    {TLV_ATTR_GROUP_OWNER,                  true},
    {TLV_ATTR_GROUP_LEVEL,                  true},
    {TLV_ATTR_GROUP_BULLETIN,               true},
    {TLV_ATTR_GROUP_END,                    true},
};                                                               

////////////////////////////////////////////////////////////////////////////////
// interface form tlv.h
////////////////////////////////////////////////////////////////////////////////
bool isAtomicTlvAttribute(int nType)
{
    if ((TLV_ATTR_TYPE_START > nType) || (nType >= TLV_ATTR_TYPE_END))
    {
        return true;
    }

    if ((TLV_ATTR_TYPE_START <= nType) && (nType <= TLV_ATTR_COMMON_END))
    {
        return tlvCommonAttrInfo_m[nType - TLV_ATTR_TYPE_START].isAtomic_m;
    }
    else if ((TLV_ATTR_USER_TYPE_START <= nType) && (nType <= TLV_ATTR_USER_END))
    {
        return tlvUserAttrInfo_m[nType - TLV_ATTR_USER_TYPE_START].isAtomic_m;
    }
    else if ((TLV_ATTR_CHAT_TYPE_START <= nType) && (nType <= TLV_ATTR_CHAT_END))
    {
        return tlvChatAttrInfo_m[nType - TLV_ATTR_CHAT_TYPE_START].isAtomic_m;
    }
    else if ((TLV_ATTR_GROUP_TYPE_START <= nType) && (nType <= TLV_ATTR_GROUP_END))
    {
        return tlvGroupAttrInfo_m[nType - TLV_ATTR_GROUP_TYPE_START].isAtomic_m;
    }
    else if ((TLV_ATTR_SERVICE_GROUP_START <= nType) && (nType <= TLV_ATTR_SERVICE_GROUP_END))
    {
        return tlvSGAttrInfo_m[nType - TLV_ATTR_SERVICE_GROUP_START].isAtomic_m;
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// interface form TlvIf.h
////////////////////////////////////////////////////////////////////////////////
TlvAttrIf_i* createTlvAttribute(int nType)
{
    TlvAttrIf_i* pAttr = NULL;

    if (isAtomicTlvAttribute(nType))
    {
        pAttr = new TlvAttrIf_i();
    }
    else
    {
        pAttr = new TlvAttrSet_c();
    }

    if (NULL != pAttr)
    {
        pAttr->setType((TlvAttrType_e)nType);
    }

    return pAttr;
}

////////////////////////////////////////////////////////////////////////////////
// TODO:
//
// the performance is bad since the content has to be copied too many times, to be
// refined
////////////////////////////////////////////////////////////////////////////////
bool TlvAttrIf_i::setValue_byte(char cValue)
{
    return TlvGenerator_c::appendByte(tbAttr_m, cValue);
}

bool TlvAttrIf_i::setValue_short(int nValue)
{
    return TlvGenerator_c::appendShort(tbAttr_m, nValue);
}

bool TlvAttrIf_i::setValue_int32(int nValue)
{
    return TlvGenerator_c::appendInt32(tbAttr_m, nValue);
}

bool TlvAttrIf_i::setValue_int64(long long lValue)
{
    return TlvGenerator_c::appendInt64(tbAttr_m, lValue);
}

bool TlvAttrIf_i::setValue_buffer(const char* sVal, int nLen)
{
    return TlvGenerator_c::appendBuf(tbAttr_m, sVal, nLen);
}

////////////////////////////////////////////////////////////////////////////////
// TODO: duplicate functions to TlvParser, to be refined
////////////////////////////////////////////////////////////////////////////////
int TlvAttrIf_i::getByte()
{
    return getInt_N(getValue(), 1);
}

int TlvAttrIf_i::getShort()
{
    return getInt_N(getValue(), TLV_LEN_TYPE_SHORT);
}

int TlvAttrIf_i::getInt32()
{
    return getInt_N(getValue(), TLV_LEN_TYPE_INT32);
}

long long TlvAttrIf_i::getInt64()
{
    const char* pBuf = getValue();

    long long nRet = 0;
    for (int nIndex=TLV_LEN_TYPE_INT64 - 1; nIndex>=0; nIndex--)
    {
        nRet <<= 8;
        nRet += pBuf[nIndex] & 0xFF;
    }

    return nRet;
}

int TlvAttrIf_i::getInt_N(const char* pBuf, int nBytes)
{
    int nRet = 0;

    for (int nIndex=nBytes - 1; nIndex>=0; nIndex--)
    {
        nRet <<= 8;
        nRet += pBuf[nIndex] & 0xFF;
    }

    return nRet;
}

const char* TlvAttrIf_i::getString()
{
    return getValue();
}
