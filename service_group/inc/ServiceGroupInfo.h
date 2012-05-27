#ifndef __SERVICE_GROUP_INFO_H_


#define __SERVICE_GROUP_INFO_H_

#include <common/inc/MyHash.h>
#include <common/inc/SimpleVector.h>

#include <interface/io/IoDataIf.h>

struct sqlite3;
class IoTransIf_i;
class TlvAttrIf_i;

class ServiceGroup_c
{
    enum ServiceGroupHostType_e
    {
        SG_HOST_TYPE_IPV4,
        SG_HOST_TYPE_IPV6,
    };

    public:
        ServiceGroup_c() :
            nServiceGroupId_m(0),
            sHost_m(NULL),
            sPort_m(NULL),
            eHostType_m(SG_HOST_TYPE_IPV4)
        {
        }

        ~ServiceGroup_c();

        void setHost(const char* sHost);
        void setPort(const char* sPort);

        inline void setId(int nId)                  { nServiceGroupId_m = nId; }
        //inline void setHostType(ServiceGroupHostType_e eType)   { eHostType_m = eType; }
        inline void setIPv4()                       { eHostType_m = SG_HOST_TYPE_IPV4; }
        inline void setIPv6()                       { eHostType_m = SG_HOST_TYPE_IPV6; }

        // for hash map
        inline int* getIdAddress()                  { return &nServiceGroupId_m; }

        inline int getId()                          { return nServiceGroupId_m; }
        inline const char* getHost()                { return sHost_m; }
        inline const char* getPort()                { return sPort_m; }

        inline bool isIPv4()                { return SG_HOST_TYPE_IPV4 == eHostType_m; }
        inline bool isIPv6()                { return SG_HOST_TYPE_IPV6 == eHostType_m; }

    protected:
    private:
        int nServiceGroupId_m;

        char* sHost_m;
        char* sPort_m;

        ServiceGroupHostType_e eHostType_m;
};

class ServiceGroupMgr_c
{
    public:
        ~ServiceGroupMgr_c();

        static ServiceGroupMgr_c* getInstance();

        inline ServiceGroup_c* getServiceGroup(int nId)     { return htSGInfo_m.lookup(nId); }

        inline void setMode(bool bMaster)                   { bMasterMode_m = bMaster; }

        void addServiceGroup(int nId);
        bool handleServiceGroupUpdate(TlvAttrIf_i* pAttr);

        bool load();
        bool save();

        //void resolveServiceGroup();

        ////////////////////////////////////////////////////////////////////////////////
        // interface IoDataIf_i
        ////////////////////////////////////////////////////////////////////////////////
        //void generateRequest(TempMemBuffer_c& tbBuffer);
        //void analyzeResponse(TempMemBuffer_c& tbBuffer);
    protected:
        ServiceGroupMgr_c();

        bool isUnderQuerying();

        bool loadSgInfo(sqlite3 *pConn);
        bool saveSgInfo(sqlite3 *pConn);
        bool insertGroup(ServiceGroup_c* pSG);
        bool updateServiceGroup(int nId, const char* sHost, const char* sPort);

        bool sendSGCheckEvent(int nSgId);
        void releaseIoTrans();

        TlvAttrIf_i* buildServiceGroupQuery(int nSGId);

        bool processSGResponse(TempSimpleVector_c<TlvAttrIf_i*>* tlvAttrs);

    private:
        // All related service group
        HashInt_c<ServiceGroup_c*> htSGInfo_m;

        TempSimpleVector_c<int>* lstPendingGroup_m;
        //IoTransIf_i* pIoTrans_m;

        bool bMasterMode_m;       // MASTER: true, SLAVE: false
};

#endif
