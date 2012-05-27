
#include <common/inc/MyDebug.h>
#include <common/inc/os_type.h>

#include <interface/baseos/AppEvent.h>
#include <interface/system/GlobalInstanceMgr.h>
#include <interface/io/IoTransIf.h>

//#include <UserService/inc/ServiceGroupInfo.h>
#include <dispatcher/inc/dispatcherInit.h>

#include <dispatcher/inc/EventDispatcher.h>

EventDispatcher_c::EventDispatcher_c()
{
}

EventDispatcher_c::~EventDispatcher_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(EventDispatcher_c)
        
void EventDispatcher_c::dispatch(AppEvent_t& event)
{
    switch (event.eEventType_m)
    {
        case EVENT_SYSTEM_INIT:
            MSG("System init...\n");
            DispatcherInit_c::getInstance()->initialize();
            break;
        case EVENT_IO_POSTPONED_DELETE:
            MSG("To delete idle io transactions...\n");
            IO_CLEAR_IDLE_TRANSACTIONS();
            break;
        case EVENT_US_SG_CHECK:
            MSG("To check service group info...\n");
            //ServiceGroupMgr_c::getInstance()->resolveServiceGroup();
            break;
        default:
            MSG("Discard unknown event [%d]\n", event.eEventType_m);
            break;
    }
}

