#ifndef __LISTENER_H_
#define __LISTENER_H_

#include <interface/listener/ListenerIf.h>

class TempMemBuffer_c;

class Listener_c
{
    public:
        Listener_c();
        ~Listener_c();

        static Listener_c* getInstance();

        bool init();

        int start(const char* sPort);

        int createSocketToConnect(const char* sServer, const char *port);
        int writeData(int nFd, TempMemBuffer_c& bufSend);

    protected:
        int createServerSocket(const char *port);

        int addPollCtrl(int nFd, bool bConnecting=false);
        int modPollCtrlToRead(int nFd);

        int makeSocketNonBlocking(int sfd);

    private:
        int nServerFd_m;

        int nPollFd_m;
};

#endif
