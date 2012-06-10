
#include <interface/listener/ListenerIf.h>

#include <listener/inc/RequestItem.h>

void RequestItem_c::release()
{
    if (0 != getSocketFd())
    {
        IO_SOCKET_CLOSE(getSocketFd());
        setSocketFd(0);
    }
#if 0

    if (NULL != getHandler())
    {
        getHandler()->unbind();
    }

    unbind();
#endif
}
