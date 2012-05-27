#ifndef __IO_TRANS_MGR_H_
#define __IO_TRANS_MGR_H_

#include <common/inc/MyHash.h>
#include <common/inc/SimpleVector.h>

class IoTransIf_i;
class IoDataIf_i;

class IoTransMgr_c
{
    public:
        ~IoTransMgr_c();

        static IoTransMgr_c* getInstance();

        IoTransIf_i* allocate(IoDataIf_i* pIoData, const char* sHost, const char* sPort);
        void release(IoTransIf_i* pIoTrans);

        IoTransIf_i* getIoTrans(int nFd);

        void releaseIdleIoTrans();

    protected:
        IoTransMgr_c();

        void addIoTrans(IoTransIf_i* pIo);
        void removeIoTrans(int nFd);

    private:
        HashInt_c<IoTransIf_i*> htIoTrans_m;

        // For postponed delete
        TempSimpleVector_c<IoTransIf_i*> lstIdleIo_m;
};

#endif
