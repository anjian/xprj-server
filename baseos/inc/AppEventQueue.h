#ifndef __APP_EVENT_QUEUE_H_
#define __APP_EVENT_QUEUE_H_

#include <interface/baseos/AppEvent.h>              // AppEvent_t
#include <interface/baseos/SoftTimer.h>             // SoftTimer interface

//#define UCWEB_APP_EVENT_SENT(event)    UcWebAppEventQueue_c::getInstance()->insertEvent(event)

class AppEventDispatchIf_i;

#if 0
struct IMessageReceiver;
struct IMessageQueue;
#endif

class AppEventQueue_c : public TimerInterface_i
{
    public:
        ~AppEventQueue_c();
        static AppEventQueue_c* getInstance();

        void setDispatcher(AppEventDispatchIf_i* dispatcher)     { pDispatcher_m = dispatcher; }

        bool init();

        void start();

        // [Event IN]
        bool insertEvent(AppEvent_t& event);
        void handleAllPendingEvent();

        ////////////////////////////////////////////////////////////////////////////////
        // TimerInterface_i: soft timer call back function
        ////////////////////////////////////////////////////////////////////////////////
        virtual void timeout(TimerEvent_t& event);

    protected:
        AppEventQueue_c();

        bool allocateEventQueue(int nSize);

        bool retrieveEvent(AppEvent_t& event);

        bool handleEvent();
        //void postIfNeed();

    private:
        AppEventDispatchIf_i* pDispatcher_m;

        // FIFO queue
        int nEventQueueSize_m;
        int nEventQueueHeader_m;
        int nEventQueueCount_m;

        AppEvent_t* pEventQueue_m;
};

#endif
