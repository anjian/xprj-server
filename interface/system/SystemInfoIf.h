#ifndef __SYSTEM_INFO_IF_H_
#define __SYSTEM_INFO_IF_H_

enum SysInfoType_e
{
    SYS_INFO_TYPE_VERSION = 1,              // pVersionStr_sm 
    
    SYS_INFO_TYPE_SERVICE_PORT,
    SYS_INFO_TYPE_SERVICE_GROUP_ID,

    SYS_INFO_TYPE_SERVICE_DATABASE,         // User Service Database

    SYS_INFO_TYPE_DI_HOST,                  // dispatcher host
    SYS_INFO_TYPE_DI_PORT,                  // dispatcher port

    // please add new type above this line
    SYS_INFO_TYPE_END
};

////////////////////////////////////////////////////////////////////////////////
// interface to get system info 
////////////////////////////////////////////////////////////////////////////////
const char * SYS_INFO_GET_STRING(SysInfoType_e eType);
int SYS_INFO_GET_INT(SysInfoType_e eType);

void SYS_INFO_SET_VALUE(SysInfoType_e eType, const char* sValue, int nLen);

bool SYS_INFO_LOAD(const char* sFile);

#endif
