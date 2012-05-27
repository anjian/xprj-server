#ifndef __APP_EVENT_DISPATCH_IF_
#define __APP_EVENT_DISPATCH_IF_

struct AppEvent_t;

class AppEventDispatchIf_i
{
    public:
        AppEventDispatchIf_i() {}
        virtual ~AppEventDispatchIf_i() {}

        virtual void dispatch(AppEvent_t& /*event*/)   {}
};

#endif
