#ifndef __TIMER_MGR_H_
#define __TIMER_MGR_H_

#include <common/inc/MyList.h>

#include <interface/baseos/SoftTimer.h>

struct TimerItem_t
{
    TimerInterface_i* obj_m;

    long long tStartTime_m;     // clock tick
    int nInterval_m;            // ms
    TimerEvent_t event_m;
};

struct TimerHelperInfo;

////////////////////////////////////////////////////////////////////////////////
// WARNNING:
//
//  It's NOT thread-safety.  All timer start/stop action MUST be done in the same thread.
//
//  If someday it's really needed to operate timer in mutli-thread, the sync 
//  mechansim should be introduced
////////////////////////////////////////////////////////////////////////////////
class SoftTimerMgr_c 
{
    public:
        static SoftTimerMgr_c* getInstance();
        ~SoftTimerMgr_c();

        bool init();

        bool startTimer(TimerInterface_i* obj, int nInterval);
        bool startTimer(TimerInterface_i* obj, int nInterval, TimerEvent_t& event);

        void stopTimerAll(TimerInterface_i* obj);
        void stopTimerAllEvent(TimerInterface_i *obj, TimerEventType_e event);

    protected:
        SoftTimerMgr_c();

        void checkTrigger(long long currentTime, int nInterval);

    private :
        static void S_fire(void* object);
        void fire();

        void stopTimerHelper(struct TimerHelperInfo *, void (SoftTimerMgr_c::*func)(struct TimerHelperInfo *));
        void stopTimerAllPtr(struct TimerHelperInfo *);
        void stopTimerAllEventPtr(struct TimerHelperInfo *);

    private:
        List_c<TimerItem_t *> lstRegisteredItemList_m;
        // put the object into pool for reuse
        List_c<TimerItem_t *> lstItemPool_m;

        long long tStartTime_m;

        //int nMinInterval_m;
};

#endif
