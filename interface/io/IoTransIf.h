#ifndef __IO_TRANS_IF_H_
#define __IO_TRANS_IF_H_

#include <interface/baseos/TempObject.h>
#include <interface/baseos/TempMemBuffer.h>

class IoDataIf_i;

class IoTransIf_i : public TempObject_i
{
    public:
        IoTransIf_i() :
            nSocketFd_m(0),
            pIoData_m(NULL)
        {
        }

        virtual ~IoTransIf_i();

        // for hash table
        inline int* getFdAddress()          { return &nSocketFd_m; }

        inline int getSocketFd()            { return nSocketFd_m; }

        inline void setSocketFd(int nFd)    { nSocketFd_m = nFd; }
        inline void setIoData(IoDataIf_i* pData)     { pIoData_m = pData; }

        bool connecting(const char* sHost, const char* sPort);

        void connected();
        void received(TempMemBuffer_c& tbReceived);

        void close();

    protected:
    private:
        int nSocketFd_m;
        IoDataIf_i* pIoData_m;

        TempMemBuffer_c tbBuffer_m;
        // TODO: to add start time for timeout handling
};

IoTransIf_i* IO_TRANSACTION_ALLOC(IoDataIf_i* pIoData, const char* sHost, const char* sPort);
void IO_TRANSACTION_RELEASE(IoTransIf_i* pIo);
void IO_CLEAR_IDLE_TRANSACTIONS();

#endif
