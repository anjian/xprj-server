#ifndef __EVENT_DISPATCHER_H_
#define __EVENT_DISPATCHER_H_

#include <interface/baseos/AppEventDispatchIf.h>

class EventDispatcher_c : public AppEventDispatchIf_i
{
    public:
        ~EventDispatcher_c();

        static EventDispatcher_c* getInstance();
        
        virtual void dispatch(AppEvent_t& event);

    protected:
        EventDispatcher_c();

    private:
};

#endif
