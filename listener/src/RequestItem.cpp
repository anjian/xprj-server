
#include <interface/listener/ListenerIf.h>

#include <listener/inc/RequestItem.h>

void RequestItem_c::release()
{
    setHandler(NULL);

    IO_SOCKET_CLOSE(getSocketFd());
}
