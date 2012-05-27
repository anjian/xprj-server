
#include <common/inc/MyDebug.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/baseos/AppEvent.h>
#include <interface/io/IoTransIf.h>

#include <io/inc/IoTransMgr.h>

////////////////////////////////////////////////////////////////////////////////
// io transaction interface operation
////////////////////////////////////////////////////////////////////////////////
IoTransIf_i* IO_TRANSACTION_ALLOC(IoDataIf_i* pIoData, const char* sHost, const char* sPort)
{
    return IoTransMgr_c::getInstance()->allocate(pIoData, sHost, sPort);
}

void IO_TRANSACTION_RELEASE(IoTransIf_i* pIo)
{
    IoTransMgr_c::getInstance()->release(pIo);
}

void IO_CLEAR_IDLE_TRANSACTIONS()
{
    IoTransMgr_c::getInstance()->releaseIdleIoTrans();
}

bool IO_SOCKET_TRANS_MANAGEABLE(int nFd)
{
    return NULL != IoTransMgr_c::getInstance()->getIoTrans(nFd);
}

IoTransIf_i* IO_SOCKET_GET_TRANS(int nFd)
{
    return IoTransMgr_c::getInstance()->getIoTrans(nFd);
}

////////////////////////////////////////////////////////////////////////////////
// IoTransMgr_c implementation
////////////////////////////////////////////////////////////////////////////////
IoTransMgr_c::IoTransMgr_c() :
    lstIdleIo_m(8)
{
}

IoTransMgr_c::~IoTransMgr_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(IoTransMgr_c)

void IoTransMgr_c::addIoTrans(IoTransIf_i* pIo)
{
    if (NULL == pIo)
    {
        return;
    }

    if (htIoTrans_m.lookup(pIo->getSocketFd()))
    {
        // TODO: to handle error case, the socket fd has been exist
    }

    MSG("Insert io transaction, socketfd=%d\n", pIo->getSocketFd());
    htIoTrans_m.insert(pIo->getFdAddress(), pIo);
}

void IoTransMgr_c::removeIoTrans(int nFd)
{
    MSG("remove io transaction, socketfd=%d\n", nFd);
    IoTransIf_i* pIo = htIoTrans_m.remove(nFd);

    if (NULL != pIo)
    {
        if (0 >= lstIdleIo_m.size())
        {
            // trigger postponed deletion
            AppEvent_t event;
            event.eEventType_m = EVENT_IO_POSTPONED_DELETE;

            APP_EVENT_SENT(event);
        }

        lstIdleIo_m.append(pIo);
    }
    else
    {
        MSG_ERR("could not find io transaction in hash table\n");
    }
}

void IoTransMgr_c::releaseIdleIoTrans()
{
    for (int nIndex=lstIdleIo_m.size()-1; nIndex>=0; nIndex--)
    {
        delete lstIdleIo_m.takeLast();
    }
}

IoTransIf_i* IoTransMgr_c::getIoTrans(int nFd)
{
    return htIoTrans_m.lookup(nFd);
}

IoTransIf_i* IoTransMgr_c::allocate(IoDataIf_i* pIoData, const char* sHost, const char* sPort)
{
    if (NULL == pIoData)
    {
        return NULL;
    }

    IoTransIf_i* pIoTrans = new IoTransIf_i;
    if (NULL == pIoTrans)
    {
        return NULL;
    }

    pIoTrans->setIoData(pIoData);

    if (!pIoTrans->connecting(sHost, sPort))
    {
        release(pIoTrans);
        return NULL;
    }

    // add into hash table
    addIoTrans(pIoTrans);

    return pIoTrans;
}

void IoTransMgr_c::release(IoTransIf_i* pIoTrans)
{
    if (NULL == pIoTrans)
    {
        return;
    }

    removeIoTrans(pIoTrans->getSocketFd());

    pIoTrans->close();
}

