//
// TODO: platform dependent, to be reorganized
//
#include <unistd.h>
#include <fcntl.h>

#include <common/inc/MyDebug.h>
#include <common/inc/MyCType.h>
#include <common/inc/bsearch.h>

#include <interface/baseos/TransientMemory.h>
#include <interface/baseos/TempMemBuffer.h>
#include <interface/system/GlobalInstanceMgr.h>

#include <system/inc/SystemInfo.h>

////////////////////////////////////////////////////////////////////////////////
// system info attribute map
////////////////////////////////////////////////////////////////////////////////
struct SysInfoMapping_t
{
    const char* s_name;
    SysInfoType_e eSysInfoType_m;
};

SysInfoMapping_t SysInfoMap_g[] =
{
    {"db",          SYS_INFO_TYPE_SERVICE_DATABASE},
    {"di-host",     SYS_INFO_TYPE_DI_HOST},
    {"di-port",     SYS_INFO_TYPE_DI_PORT},
    {"port",        SYS_INFO_TYPE_SERVICE_PORT},
    {"sgid",        SYS_INFO_TYPE_SERVICE_GROUP_ID},
};

const int N_SYS_INFO = sizeof(SysInfoMap_g)/sizeof(SysInfoMap_g[0]);



////////////////////////////////////////////////////////////////////////////////
// interface to get system info 
////////////////////////////////////////////////////////////////////////////////
const char * SYS_INFO_GET_STRING(SysInfoType_e eType)
{
    return SystemInfo_c::getInstance()->getInfo(eType);
}

int SYS_INFO_GET_INT(SysInfoType_e eType)
{
    return 0;
}

void SYS_INFO_SET_VALUE(SysInfoType_e eType, const char* sValue, int nLen)
{
    SystemInfo_c::getInstance()->appendAttribute(eType, sValue, nLen);
}

bool SYS_INFO_LOAD(const char* sFile)
{
    return SystemInfo_c::getInstance()->load(sFile);
}

//
////////////////////////////////////////////////////////////////////////////////

SystemInfo_c::SystemInfo_c() :
    pTempBuf_m(NULL)
{
}

SystemInfo_c::~SystemInfo_c()
{
    if (NULL != pTempBuf_m)
    {
        delete pTempBuf_m;
    }
}

IMPLE_OBJECT_GET_INSTANCE(SystemInfo_c)

bool SystemInfo_c::init()
{
    return true;
}

const char* SystemInfo_c::getInfo(SysInfoType_e eType)
{
    SysInfoAttr_t* pAttr = htSysInfo_m.lookup(eType);

    return (NULL != pAttr) ? pAttr->sValue_m : NULL;
}

bool SystemInfo_c::load(const char* sFile)
{
    if (NULL == sFile)
    {
        return false;
    }

    int nBufLen = 1024;
    char* pBuffer = (char*)TR_MEMORY_ALLOC(nBufLen);
    if (NULL == pBuffer)
    {
        MSG("memory error\n");
        return false;
    }

    int nFd = open(sFile, O_RDONLY);
    if (nFd <= 0)
    {
        MSG_ERR("Could not open config file: %s\n", sFile);
        return false;
    }

    int nReadLen = 0;
    while ((nReadLen = read(nFd, pBuffer, nBufLen)) > 0)
    {
        // analyze
        if (!analyze(pBuffer, nReadLen))
        {
            return false;
        }
    }

    if ((NULL != pTempBuf_m) && (pTempBuf_m->getLength() > 0))
    {
        pBuffer[0] = '\n';
        analyze(pBuffer, 1);
    }

    close(nFd);
    TR_MEMORY_FREE(pBuffer);

    if (NULL != pTempBuf_m)
    {
        delete pTempBuf_m;
        pTempBuf_m = NULL;
    }

    return true;
}

bool SystemInfo_c::analyze(const char* sContent, int nLen)
{
    const char* sConf = sContent;
    int nConfLen = nLen;

    if ((NULL != pTempBuf_m) && (pTempBuf_m->getLength() > 0))
    {
        pTempBuf_m->append(sContent, nLen);

        sConf   = pTempBuf_m->getBuffer();
        nConfLen= pTempBuf_m->getLength();
    }

    for (;;)
    {
        nConfLen -= skipSpace(&sConf, nConfLen);
        if (0 >= nConfLen)
        {
            if (NULL != pTempBuf_m)
            {
                pTempBuf_m->clear();
            }

            return true;
        }

        const char* pEOL = getEOL(sConf, nConfLen);
        if (NULL == pEOL)
        {
            // cache for next round
            if (NULL == pTempBuf_m)
            {
                pTempBuf_m = new TempMemBuffer_c;
                if (NULL == pTempBuf_m)
                {
                    return false;
                }
            }

            pTempBuf_m->clear();
            pTempBuf_m->append(sConf, nConfLen);

            break;
        }
        else
        {
            analyzeAttribute(sConf, pEOL - sConf);

            nConfLen -= (pEOL - sConf);
            sConf = pEOL;
        }
    }

    return true;
}

bool SystemInfo_c::analyzeAttribute(const char* sAttribute, int nLen)
{
    if ((NULL == sAttribute) || (0 >= nLen))
    {
        return false;
    }

    if ('#' == sAttribute[0])
    {
        // comment, return directly
        return true;
    }

    // 1. find the key
    int nNameLen = 0;
    for (nNameLen=0; nNameLen<nLen; nNameLen++)
    {
        if (x_isspace(sAttribute[nNameLen]) || ('=' == sAttribute[nNameLen]))
        {
            break;
        }
    }

    if ((0 == nNameLen) || (nNameLen >= nLen))
    {
        return false;
    }

    // 2. get key id
    int nAttrIndex = b_search<SysInfoMapping_t>(SysInfoMap_g, N_SYS_INFO, sAttribute, nNameLen);
    if (-1 == nAttrIndex)
    {
        // could not find attribute
        MSG_ERR("invalid attribute info: %s", sAttribute);
        return false;
    }

    // 3. get the value start position
    const char* sAttrValue = sAttribute + nNameLen;

    // 3.1 jump the equal sign
    bool bFoundHeader = false;
    int nLeftLen = nLen - nNameLen;
    for (int nPos=0; nPos<nLeftLen; nPos++)
    {
        if (!x_isspace(sAttrValue[nPos]) && ('=' != sAttrValue[nPos]))
        {
            bFoundHeader = true;
            sAttrValue += nPos;

            nLeftLen -= nPos;

            break;
        }
    }

    if (!bFoundHeader)
    {
        return false;
    }

    // 3.2 get value length
    int nValueLen = 0;
    for (int nPos=0; nPos<nLeftLen; nPos++)
    {
        if (x_isspace(sAttrValue[nPos]) || ('#' == sAttrValue[nPos]))
        {
            break;
        }
        else
        {
            nValueLen++;
        }
    }

    if (0 >= nValueLen)
    {
        MSG_ERR("illegal attribute value\n");
        return false;
    }

    return appendAttribute(SysInfoMap_g[nAttrIndex].eSysInfoType_m, sAttrValue, nValueLen);
}

bool SystemInfo_c::appendAttribute(int nType, const char* sValue, int nLen)
{
    SysInfoAttr_t* pAttr = htSysInfo_m.lookup(nType);
    if (NULL == pAttr)
    {
        pAttr = new SysInfoAttr_t;
        if (NULL == pAttr)
        {
            MSG_ERR("could not allocate system info attribute item\n");
            return false;
        }

        pAttr->nId_m = nType;

        htSysInfo_m.insert(&pAttr->nId_m, pAttr);
    }

    if (NULL != pAttr->sValue_m)
    {
        delete[] pAttr->sValue_m;
        pAttr->sValue_m = NULL;
    }

    pAttr->sValue_m = new char[nLen + 1];
    if (NULL == pAttr->sValue_m)
    {
        MSG_ERR("could not allocate system info attribute value\n");
        return false;
    }

    xmemcpy(pAttr->sValue_m, sValue, nLen);
    pAttr->sValue_m[nLen] = 0;

    MSG("Set System Info: [%d]=%s\n", pAttr->nId_m, pAttr->sValue_m);

    return true;
}

int SystemInfo_c::skipSpace(const char** sConf, int nLen)
{
    int nSpaceNum=0;
    for (nSpaceNum=0; nSpaceNum<nLen; nSpaceNum++)
    {
        if (x_isspace(**sConf))
        {
            (*sConf)++;
        }
        else
        {
            break;
        }
    }

    return nSpaceNum;
}

const char* SystemInfo_c::getEOL(const char* sConf, int nConfLen)
{
    const char* sEOL = NULL;
    for (int nIndex=0; nIndex<nConfLen; nIndex++)
    {
        if (('\r' == sConf[nIndex]) || ('\n' == sConf[nIndex]))
        {
            sEOL = sConf + nIndex;
            break;
        }
    }

    return sEOL;
}
