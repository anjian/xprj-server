#ifndef __SYSTEM_INFO_H_
#define __SYSTEM_INFO_H_

#include <common/inc/MyHash.h>

#include <interface/system/SystemInfoIf.h>

class TempMemBuffer_c;

class SystemInfo_c
{
    struct SysInfoAttr_t
    {
        SysInfoAttr_t() :
            nId_m(0),
            sValue_m(NULL)
        {
        }

        ~SysInfoAttr_t()
        {
            if (NULL != sValue_m)
            {
                delete[] sValue_m;
            }
        }

        int nId_m;
        char* sValue_m;
    };

    public:
        ~SystemInfo_c();

        static SystemInfo_c* getInstance();

        bool init();
        bool load(const char* sFile);

        bool appendAttribute(int nType, const char* sValue, int nLen);

        const char* getInfo(SysInfoType_e eType);

    protected:
        SystemInfo_c();

        bool analyze(const char* sContent, int nLen);
        bool analyzeAttribute(const char* sAttribute, int nLen);
        int skipSpace(const char** sConf, int nLen);
        const char* getEOL(const char* sConf, int nConfLen);

    private:
        TempMemBuffer_c* pTempBuf_m;

        HashInt_c<SysInfoAttr_t*> htSysInfo_m;
};

#endif
