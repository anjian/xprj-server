#include <common/inc/MyDebug.h>
#include <common/inc/MyHash.h>
#include <common/inc/MyStdLibrary.h>

#include <interface/baseos/TempMemBuffer.h>
#include <interface/listener/ListenerIf.h>
#include <interface/data_proxy/DataProxyIf.h>
#include <interface/baseos/AppEvent.h>
#include <interface/baseos/SoftTimer.h>
#include <interface/system/SystemInfoIf.h>

#include <UserService/inc/EventDispatcher.h>
#include <UserService/inc/RequestHandler.h>
#include <UserService/inc/MessageHandler.h>


bool startBaseService()
{
    if (false == SOFT_TIMER_MGR_START())
    {
        MSG_ERR("starting soft timer manager: ERROR\n");
        return false;
    }

    if (false == APP_EVENT_QUEUE_START())
    {
        MSG_ERR("starting app event queue: ERROR\n");
        return false;
    }

    // set app event dispatcher
    APP_EVENT_SET_DISPATCHER(EventDispatcher_c::getInstance());

    return true;
}

//#define USER_SERVICE_PORT      "8082"

int main(int argc, char* argv[])
{
    MSG("Service Starting: User Service.\n");
    //userTest();
    //
    const char* sServicePort      = NULL;
    const char* sServiceGroupId   = NULL;
    const char* sDataBase         = NULL;
    const char* sConfFile         = NULL;
    for(int nIndex=0; nIndex<argc; nIndex++)
    {
        if (0 == xstricmp(argv[nIndex], "-p"))
        {
            if (nIndex + 1 < argc)
            {
                nIndex++;
                sServicePort = argv[nIndex];
            }
            else
            {
                MSG_ERR("USAGE: missing service port\n");
                return 1;
            }
        }
        else if (0 == xstricmp(argv[nIndex], "-db"))
        {
            if (nIndex + 1 < argc)
            {
                nIndex++;
                sDataBase = argv[nIndex];
            }
            else
            {
                MSG_ERR("USAGE: missing database name\n");
                return 1;
            }
        }
        else if (0 == xstricmp(argv[nIndex], "-sg"))
        {
            if (nIndex + 1 < argc)
            {
                nIndex++;
                sServiceGroupId = argv[nIndex];
            }
            else
            {
                MSG_ERR("USAGE: missing service group id\n");
                return 1;
            }
        }
        else if (0 == xstricmp(argv[nIndex], "-f"))
        {
            if (nIndex + 1 < argc)
            {
                nIndex++;
                sConfFile = argv[nIndex];
            }
            else
            {
                MSG_ERR("USAGE: missing configuration file name\n");
                return 1;
            }
        }
    }

#if 0
    if ((0 >= nServicePort)
            || (NULL == sDataBase)
            || (0 >= nServiceGroupId)
            )
#endif
    if (NULL == sConfFile)
    {
        MSG("USAGE: UserService -f [configration file] -db [database name]\n");
        return 1;
    }

    if (!SYS_INFO_LOAD(sConfFile))
    {
        MSG_ERR("could not load system configratuion\n");
        return 1;
    }

    // update system info from command line
    {
        if (NULL != sServicePort)
        {
            SYS_INFO_SET_VALUE(SYS_INFO_TYPE_SERVICE_PORT, sServicePort, xstrlen(sServicePort));
        }

        if (NULL != sServiceGroupId)
        {
            SYS_INFO_SET_VALUE(SYS_INFO_TYPE_SERVICE_GROUP_ID, sServiceGroupId, xstrlen(sServiceGroupId));
        }

        if (NULL != sDataBase)
        {
            SYS_INFO_SET_VALUE(SYS_INFO_TYPE_SERVICE_DATABASE, sDataBase, xstrlen(sDataBase));
        }
    }

    if (!startBaseService())
    {
        MSG_ERR("encounter error while starting base service\n");
        return 1;
    }

    {
        // trigger async system init event
        AppEvent_t event;
        event.eEventType_m = EVENT_SYSTEM_INIT;

        APP_EVENT_SENT(event);
    }

    SET_SERVER_REQUEST_HANDLER(clientRequestHandler);
    SET_CLIENT_RESPONSE_HANDLER(serverResponseHandler);

    IO_SOCKET_START_LISTENER(SYS_INFO_GET_STRING(SYS_INFO_TYPE_SERVICE_PORT));

    return 0;
}

