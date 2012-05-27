#ifndef __NET_DATA_PROXY_H_
#define __NET_DATA_PROXY_H_

#include <common/inc/MyHash.h>

#include <interface/baseos/TempMemBuffer.h>

#include <interface/io/IoDataIf.h>
#include <interface/io/IoTransIf.h>

class NetDataNode_c : public IoDataIf_i
{
    public:
        NetDataNode_c();
        ~NetDataNode_c();

        inline void setSgId(int nId)        { nSgId_m = nId; }
        inline int* getIdAddress()          { return &nSgId_m; }

        void appendRequest(const char* pReq, int nLen);

        void start();

        ////////////////////////////////////////////////////////////////////////////////
        // interface IoDataIf_i
        ////////////////////////////////////////////////////////////////////////////////
        //virtual void generateRequest(TempMemBuffer_c& tbBuffer);
        virtual TempMemBuffer_c* retrieveRequestContent();
        virtual void analyzeResponse(TempMemBuffer_c& tbBuffer);

    protected:
        void allocateIoTrans();
        void releaseIoTrans();

        bool hasNewData();
        bool isIOWorking();

    private:
        int nSgId_m;
        IoTransIf_i* pIoTrans_m;

        // There are two buffers for message, one is used to append new incoming message
        // to service group, while the other is handling IO process
        int nAppendingIndex_m;
        TempMemBuffer_c tbBuffer_m[2];
};

class NetDataProxy_c
{
    public:
        ~NetDataProxy_c();
        
        static NetDataProxy_c* getInstance();

        bool insertRequest(int nSgId, const char* pBuf, int nLen);

        void releaseNode(int nSgId);

    protected:
        NetDataProxy_c();

        void deleteIdleNodes();

    private:
        HashInt_c<NetDataNode_c*> htNodes_m;

        TempSimpleVector_c<NetDataNode_c*> lstIdleNodes_m;
};

#endif
