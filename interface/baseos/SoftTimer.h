#ifndef _UC_TIMER_H_
#define _UC_TIMER_H_


enum TimerEventType_e
{
    TIMER_EVENT_START = 0,
    TIMER_EVENT_UNKNOWN = TIMER_EVENT_START,

    // Please add new type above this line
    WIDGET_TIMER_EVENT_END
};

struct TimerEvent_t
{
    TimerEventType_e eType_m;

#if 0
    union
    {
        // info for needed event type
    };
#endif
};

// The interface of timer
class TimerInterface_i
{
    public:
        virtual ~TimerInterface_i() {}

        // timeout call back function
        virtual void timeout(TimerEvent_t& /*eEvent*/)      {}
};

////////////////////////////////////////////////////////////////////////////////
// interface to start/stop timer
////////////////////////////////////////////////////////////////////////////////
bool SOFT_TIMER_MGR_START();

bool TIMER_START(TimerInterface_i* obj, int nInterval);
bool TIMER_START(TimerInterface_i* obj, int nInterval, TimerEvent_t& event);
void TIMER_STOP_ALL(TimerInterface_i* obj);
void TIMER_STOP_ALL_EVENT(TimerInterface_i *obj, TimerEventType_e event);

#endif
