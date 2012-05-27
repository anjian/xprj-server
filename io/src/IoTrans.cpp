
#include <common/inc/MyDebug.h>

#include <interface/baseos/TempMemBuffer.h>
#include <interface/listener/ListenerIf.h>

#include <interface/protocol/TlvIf.h>

#include <interface/io/IoTransIf.h>
#include <interface/io/IoDataIf.h>

IoTransIf_i::~IoTransIf_i()
{
    if (nSocketFd_m > 0)
    {
        close();
    }
}

bool IoTransIf_i::connecting(const char* sHost, const char* sPort)
{
    nSocketFd_m = IO_SOCKET_CREATE(sHost, sPort);

    return (0 < nSocketFd_m);
}

void IoTransIf_i::connected()
{
    MSG("socket is connected, prepare send data...\n");
    if (NULL == pIoData_m)
    {
        return;
    }

    TempMemBuffer_c* tbBuffer = pIoData_m->retrieveRequestContent();
    MSG("[%d] data retrieved, writing data into socket...\n", tbBuffer->getLength());

    // write data
    IO_SOCKET_WRITE(nSocketFd_m, *tbBuffer);
}

void IoTransIf_i::received(TempMemBuffer_c& tbReceived)
{
    if (NULL == pIoData_m)
    {
        return;
    }

    MSG("response [%d] received\n", tbReceived.getLength());

    if (0 >= tbBuffer_m.getLength())
    {
        if (TlvParser_c::isMessageCompleted(tbReceived.getBuffer(), tbReceived.getLength()))
        {
            pIoData_m->analyzeResponse(tbReceived);
        }
        else
        {
            MSG("response [%d] received, continue waiting left\n", tbReceived.getLength());
            tbBuffer_m.append(tbReceived.getBuffer(), tbReceived.getLength());
        }
    }
    else
    {
        tbBuffer_m.append(tbReceived.getBuffer(), tbReceived.getLength());
        if (TlvParser_c::isMessageCompleted(tbBuffer_m.getBuffer(), tbBuffer_m.getLength()))
        {
            pIoData_m->analyzeResponse(tbBuffer_m);
        }
    }
}

void IoTransIf_i::close()
{
    MSG("close io socket fd [%d]\n", nSocketFd_m);
    if (nSocketFd_m >= 0)
    {
        IO_SOCKET_CLOSE(nSocketFd_m);

        nSocketFd_m = 0;
    }
}
