
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <errno.h>

#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>

#include <interface/baseos/MemoryIf.h>
#include <interface/baseos/TempMemBuffer.h>
#include <interface/listener/ListenerIf.h>
#include <interface/system/GlobalInstanceMgr.h>
#include <interface/io/IoTransIf.h>

#include <listener/inc/RequestItemMgr.h>
#include <listener/inc/listener.h>

#define POOL_MAX_EVENTS    64

////////////////////////////////////////////////////////////////////////////////
// ListerIf.h interface implementation
////////////////////////////////////////////////////////////////////////////////
int IO_SOCKET_START_LISTENER(const char* sPort)
{
    return Listener_c::getInstance()->start(sPort);
}

int IO_SOCKET_CREATE(const char* sHost, const char* sPort)
{
    return Listener_c::getInstance()->createSocketToConnect(sHost, sPort);
}

int IO_SOCKET_WRITE(int nFd, TempMemBuffer_c& tbSend)
{
    return Listener_c::getInstance()->writeData(nFd, tbSend);
}

void IO_SOCKET_CLOSE(int nFd)
{
    MSG("close socket [%d]\n", nFd);
    close(nFd);
}

////////////////////////////////////////////////////////////////////////////////
// Listener
////////////////////////////////////////////////////////////////////////////////
Listener_c::Listener_c()
{
}

Listener_c::~Listener_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(Listener_c)

bool Listener_c::init()
{
    return true;
}

int Listener_c::createServerSocket(const char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    xmemset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family     = AF_UNSPEC;    /* Return IPv4 and IPv6 choices */
    hints.ai_socktype   = SOCK_STREAM;  /* We want a TCP socket */
    hints.ai_flags      = AI_PASSIVE;   /* All interfaces */

    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0)
    {
        MSG_ERR("getaddrinfo: %s\n", gai_strerror (s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
        {
            continue;
        }

        s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
        if (s == 0)
        {
            /* We managed to bind successfully! */
            break;
        }

        close (sfd);
    }

    freeaddrinfo(result);

    if (rp == NULL)
    {
        MSG_ERR("Could not bind\n");
        return -1;
    }

    return sfd;
}

int Listener_c::createSocketToConnect(const char* sServer, const char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    xmemset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family     = AF_UNSPEC;    /* Return IPv4 and IPv6 choices */
    hints.ai_socktype   = SOCK_STREAM;  /* We want a TCP socket */

    /*
    hints.ai_family     = AF_UNSPEC;        // Allow IPv4 or IPv6
    hints.ai_socktype   = SOCK_DGRAM;       // Datagram socket
    hints.ai_flags      = AI_PASSIVE;       // For wildcard IP address, seems for server
    hints.ai_protocol   = 0;                // Any protocol
    hints.ai_canonname  = NULL;
    hints.ai_addr       = NULL;
    hints.ai_next       = NULL;
    */


    s = getaddrinfo (sServer, port, &hints, &result);
    if (s != 0)
    {
        MSG_ERR("getaddrinfo: get host (%s) error [%s]\n", sServer, gai_strerror (s));
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
        {
            MSG_ERR("create socket error %s:%s\n", sServer, port);
            continue;
        }

        // make socket non-block
        if (-1 == makeSocketNonBlocking(sfd))
        {
            MSG_ERR("make socket non-blocking error %d\n", sfd);
            close(sfd);

            continue;
        }

        s = connect(sfd, rp->ai_addr, rp->ai_addrlen);
        int err = errno;
        if (s < 0 /*&& err != EAGAIN*/ && err != EINPROGRESS)
        {
            // encounter error
            MSG_ERR("connect error err = %d\n", err);
            close (sfd);
        }
        else
        {
            if (0 == s)
            {
                // TODO: connection is established
                MSG("connection established\n");
            }

            // connect is in progress
            break;
        }
    }

    freeaddrinfo(result);

    if (rp == NULL)
    {
        MSG_ERR("Could not connect\n");
        return -1;
    }

    addPollCtrl(sfd, true);
    MSG("connection is under establishing...\n");

    return sfd;
}


int Listener_c::makeSocketNonBlocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        MSG_ERR("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        MSG_ERR("fcntl");
        return -1;
    }

    return 0;
}

int Listener_c::writeData(int nFd, TempMemBuffer_c& bufSend)
{
    // Write the message
    ssize_t nWritted = 0;
    while (nWritted < bufSend.getLength())
    {
        ssize_t nWriteCount = write(nFd, bufSend.getBuffer() + nWritted, bufSend.getLength() - nWritted);
        if (nWriteCount == -1)
        {
            // TODO: IO error handling
            //if (errno != EAGAIN)
            //{
            //}
            MSG_ERR("socket write error fd [%d]\n", nFd);

            return -1;
        }
        else
        {
            nWritted += nWriteCount;
        }
    }
    
    return 0;
}

int Listener_c::addPollCtrl(int nFd, bool bConnecting)
{
    struct epoll_event event;

    event.data.fd   = nFd;

    if (bConnecting)
    {
        event.events    = EPOLLIN | EPOLLOUT;
    }
    else
    {
        event.events    = EPOLLIN;
    }

    //event.events = EPOLLIN | EPOLLET;
    //
    int nRet = epoll_ctl(nPollFd_m, EPOLL_CTL_ADD, nFd, &event);
    if (nRet == -1)
    {
        MSG_ERR("epoll_ctl error [%d]\n", nFd);
        //perror ("epoll_ctl");
        return -1;
    }

    return 0;
}

int Listener_c::modPollCtrlToRead(int nFd)
{
    //  don't want output anymore
    struct epoll_event event;

    event.data.fd   = nFd;
    event.events    = EPOLLIN;

    int nRet = epoll_ctl(nPollFd_m, EPOLL_CTL_MOD, nFd, &event);
    if (nRet == -1)
    {
        MSG_ERR("epoll_ctl mod error [%d]\n", nFd);
        //perror ("epoll_ctl");
        return -1;
    }

    return 0;
}

int Listener_c::start(const char* sPort)
{
    int s;
    //struct epoll_event event;
    struct epoll_event *events;

    MSG("start listener @ port %s\n", sPort);

    nServerFd_m = createServerSocket(sPort);
    if (nServerFd_m == -1)
    {
        return -1;
    }

    s = makeSocketNonBlocking(nServerFd_m);
    if (s == -1)
    {
        return -1;
    }

    s = listen(nServerFd_m, SOMAXCONN);
    if (s == -1)
    {
        MSG_ERR("listen error, exit now\n");
        //perror ("listen");
        return -1;
    }

    nPollFd_m = epoll_create1(0);
    if (nPollFd_m == -1)
    {
        MSG_ERR("epoll_create error, exit now\n");
        //perror ("epoll_create");
        return -1;
    }

#if 0
    event.data.fd   = nServerFd_m;
    event.events    = EPOLLIN;
    //event.events = EPOLLIN | EPOLLET;
    //event.events = EPOLLIN | EPOLLOUT;                  // edge trigger
    s = epoll_ctl(nPollFd_m, EPOLL_CTL_ADD, nServerFd_m, &event);
    if (s == -1)
    {
        MSG_ERR("epoll_ctl error, exit now\n");
        //perror ("epoll_ctl");
        return -1;
    }
#endif
    if (-1 == addPollCtrl(nServerFd_m))
    {
        MSG_ERR("epoll_ctl error, exit now\n");
        return -1;
    }

    // Buffer where events are returned
    events = (epoll_event *)xmalloc(POOL_MAX_EVENTS * sizeof(epoll_event));

    // The event loop
    int nEvtCount = 0;
    for (;;)
    {
        do
        {
            nEvtCount = epoll_wait(nPollFd_m, events, POOL_MAX_EVENTS, -1);
        } while (nEvtCount < 0 && errno == EINTR);

        MSG("epoll is tiggered, event count:%d\n", nEvtCount);
        if (nEvtCount < 0)
        {
            switch (errno)
            {
                case EBADF:
                    MSG("EBADF\n");
                    break;
                case EFAULT:
                    MSG("EFAULT\n");
                    break;
                case EINTR:
                    MSG("EINTR\n");
                    break;
                case EINVAL:
                    MSG("EINVAL\n");
                    break;
                default:
                    MSG("unknown\n");
            }
        }

        for (int nEvtIndex=0; nEvtIndex<nEvtCount; nEvtIndex++)
        {
            if ((events[nEvtIndex].events & EPOLLERR) ||
                    (events[nEvtIndex].events & EPOLLHUP) ||
                    (!((events[nEvtIndex].events & EPOLLIN) || (events[nEvtIndex].events & EPOLLOUT)))
                    )
            {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                MSG_ERR("epoll error\n");

                close(events[nEvtIndex].data.fd);

                IoTransIf_i* pIo = IO_SOCKET_GET_TRANS(events[nEvtIndex].data.fd);
                if (NULL == pIo)
                {
                    RequestItemMgr_c::getInstance()->releaseReqItem(events[nEvtIndex].data.fd);
                }
                else
                {
                    IO_TRANSACTION_RELEASE(pIo);
                }

                continue;
            }

#if 0
            if (events[nEvtIndex].events & EPOLLET)
            {
                printf("epollet\n");
            }
            if (events[nEvtIndex].events & EPOLLIN)
            {
                printf("epoll IN\n");
            }
            if (events[nEvtIndex].events & EPOLLOUT)
            {
                printf("epoll OUT\n");
            }
#endif

            if (nServerFd_m == events[nEvtIndex].data.fd)
            {
                // We have a notification on the listening socket, which
                // means one or more incoming connections.
                for (;;)
                {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;

                    in_len = sizeof in_addr;
                    infd = accept (nServerFd_m, &in_addr, &in_len);
                    if (infd == -1)
                    {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                        {
                            // We have processed all incoming connections.
                            break;
                        }
                        else
                        {
                            MSG_ERR("accept socket error");
                            //perror ("accept");
                            break;
                        }
                    }

                    // debug
                    {
                        char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
                        s = getnameinfo (&in_addr, in_len,
                                hbuf, sizeof hbuf,
                                sbuf, sizeof sbuf,
                                NI_NUMERICHOST | NI_NUMERICSERV);
                        if (s == 0)
                        {
                            MSG("Accepted connection on descriptor %d " "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                        }
                    }

                    // Make the incoming socket non-blocking and add it to the list of fds to monitor.
                    s = makeSocketNonBlocking(infd);
                    if (s == -1)
                    {
                        MSG_ERR("Could not make non-block socket, close it [%d]\n", infd);
                        //abort ();
                    }

                    addPollCtrl(infd);
#if 0
                    event.data.fd = infd;
                    event.events = EPOLLIN;
                    //event.events = EPOLLIN | EPOLLET;
                    s = epoll_ctl (nPollFd_m, EPOLL_CTL_ADD, infd, &event);
                    if (s == -1)
                    {
                        MSG_ERR("epoll_ctrl error: could not add socket [%d] into pool\n", infd);
                        //abort ();
                    }
#endif
                }

                continue;
            }
            else if (events[nEvtIndex].events & EPOLLOUT)
            {
                //  take this as a hint that we've connected
                //  notify user the socket is ready to write
                
                //  don't want output anymore
                modPollCtrlToRead(events[nEvtIndex].data.fd);

                IoTransIf_i* pIo = IO_SOCKET_GET_TRANS(events[nEvtIndex].data.fd);
                if (NULL != pIo)
                {
                    pIo->connected();
                }
                else
                {
                    // error case
                }
                          // char const *buf = "GET / HTTP/1.1\r\nHost: google.com\r\nConnection: close\r\n\r\n";
                          // size_t len = strlen(buf);
                          // size_t sent = 0;
                          // while (sent < len) {
                          // r = send(fd, buf, len-sent, 0);
                          // if (r < 1) {
                          // fprintf(stderr, "send() returns %d; exit\n", r);
                          // exit(1);
                          // }
                          // sent += r;
                          // }
                          // ev.events = EPOLLIN;
                          // ev.data.fd = fd;
                          // epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
                          // }
                          //
            }
            else
            {
                // We have data on the fd waiting to be read. Read and
                // display it. We must read whatever data is available
                // completely, as we are running in edge-triggered mode
                // and won't get a notification again for the same data.
                int done = 0;

                TempMemBuffer_c bufReceived;
                TempMemBuffer_c bufSend;

                while (1)
                {
                    ssize_t count;
                    char buf[512];

                    count = read(events[nEvtIndex].data.fd, buf, sizeof buf);
                    if (count == -1)
                    {
                        // If errno == EAGAIN, that means we have read all
                        // data. So go back to the main loop.
                        if (errno != EAGAIN)
                        {
                            MSG_ERR("Read scoket error [%d], to close socket\n", events[nEvtIndex].data.fd);
                            //perror ("read");
                            done = 1;
                        }
                        break;
                    }
                    else if (count == 0)
                    {
                        // End of file. The remote has closed the
                        // connection.
                        done = 1;
                        break;
                    }

                    bufReceived.append(buf, count);
#if 0
                    // Write the buffer to standard output
                    s = write (1, buf, count);
                    if (s == -1)
                    {
                        perror ("write");
                        abort ();
                    }
#endif
                }

                IoTransIf_i* pIo = IO_SOCKET_GET_TRANS(events[nEvtIndex].data.fd);
                if (NULL == pIo)
                {
                    // server side
                    if (bufReceived.getLength() > 0)
                    {
                        bool bProcessed = RequestItemMgr_c::getInstance()->handleRequest(events[nEvtIndex].data.fd, bufReceived);

                        if (!bProcessed)
                        {
                            MSG("error occurred while handling request, release fd [%d]\n", events[nEvtIndex].data.fd);

                            /* Closing the descriptor will make epoll remove it
                               from the set of descriptors which are monitored. */
                            //close(events[nEvtIndex].data.fd);
                        }
                        else
                        {
                            RequestItemMgr_c::getInstance()->releaseReqItem(events[nEvtIndex].data.fd);
                        }
                        // Write the response back
                        //writeData(events[nEvtIndex].data.fd, bufSend);
                    }
                    else
                    {
                        RequestItemMgr_c::getInstance()->releaseReqItem(events[nEvtIndex].data.fd);
                    }
                }
                else
                {
                    // As client, got server response 
                    pIo->received(bufReceived);
                }

                if (done)
                {
                    MSG("Close connection on descriptor fd [%d]\n", events[nEvtIndex].data.fd);

                    /* Closing the descriptor will make epoll remove it
                       from the set of descriptors which are monitored. */
                    close(events[nEvtIndex].data.fd);
                }
            }
        }
    }

    xfree(events);

    close(nServerFd_m);

    return 0;//EXIT_SUCCESS;
}
