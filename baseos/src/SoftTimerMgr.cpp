//#include <iostream>

#include <common/inc/MyDebug.h>
#include <common/inc/MyTime.h>
#include <common/inc/MySystem.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/platform/TimerOp.h>
//#include <interface/TimerOp.h>

#include <baseos/inc/SoftTimerMgr.h>

////////////////////////////////////////////////////////////////////////////////
// interface implementation
////////////////////////////////////////////////////////////////////////////////
//
bool SOFT_TIMER_MGR_START()
{
    return SoftTimerMgr_c::getInstance()->init();
}

bool TIMER_START(TimerInterface_i* obj, int nInterval)
{
    return SoftTimerMgr_c::getInstance()->startTimer(obj, nInterval);
}

bool TIMER_START(TimerInterface_i* obj, int nInterval, TimerEvent_t& event)
{
    return SoftTimerMgr_c::getInstance()->startTimer(obj, nInterval, event);
}

void TIMER_STOP_ALL(TimerInterface_i* obj)
{
    SoftTimerMgr_c::getInstance()->stopTimerAll(obj);
}

void TIMER_STOP_ALL_EVENT(TimerInterface_i *obj, TimerEventType_e event)
{
    SoftTimerMgr_c::getInstance()->stopTimerAllEvent(obj, event);
}

struct TimerHelperInfo
{
    TimerInterface_i * pObj_m;
    TimerItem_t * pItem_m;

    int nIndex_m;
};

//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// TODO: this function can be accessed by multi-thread, mutex needed to protect
////////////////////////////////////////////////////////////////////////////////
//
SoftTimerMgr_c::SoftTimerMgr_c() :
    tStartTime_m(0L)
{
}

SoftTimerMgr_c::~SoftTimerMgr_c()
{
    for (int nIndex=lstRegisteredItemList_m.size() - 1; nIndex>=0; nIndex--)
    {
        // Remove item
        TimerItem_t* pItem = lstRegisteredItemList_m.takeLast();
        if (NULL != pItem)
        {
            delete pItem;
        }
    }

    for (int nIndex=lstItemPool_m.size() - 1; nIndex>=0; nIndex--)
    {
        // Remove item
        TimerItem_t* pItem = lstItemPool_m.takeLast();
        if (NULL != pItem)
        {
            delete pItem;
        }
    }

    // destroy system timer
    const struct TimerOp_t * psTimerOp = get_x_timer_op();
    psTimerOp->destroy();
}

IMPLE_OBJECT_GET_INSTANCE(SoftTimerMgr_c)

bool SoftTimerMgr_c::init()
{
    const struct TimerOp_t * psTimerOp = get_x_timer_op();
    if (!psTimerOp->create_timer())
    {
        return false;
    }

    psTimerOp->set_object_notify(S_fire);
    psTimerOp->set_private_data(this);

    return true;
}

void SoftTimerMgr_c::checkTrigger(long long currentTime, int nInterval)
{
    //MSG("Timer Manager: start to check...\n");

    const struct TimerOp_t * psTimerOp = get_x_timer_op();

    // Check if the timer need to restart
    if (false == psTimerOp->is_active())
    {
        psTimerOp->start(nInterval);
        tStartTime_m = currentTime;

        //MSG("start timer with interval %d\n", nInterval);
    }
    else
    {
        // if registerred list is not empty, that means the timer has been started, check current
        // wait interval to see whether the timer need restarting
        int nLeftInterval = psTimerOp->get_interval() - (currentTime - tStartTime_m) ;

        if (nLeftInterval > nInterval)
        {
            psTimerOp->stop();

            psTimerOp->start(nInterval);
            //pTimer_m->setInterval(nInterval);
            tStartTime_m    = currentTime;

            //MSG("change interval from %d to %d\n", nLeftInterval, nInterval);
        }
    }
}

bool SoftTimerMgr_c::startTimer(TimerInterface_i* obj, int nInterval)
{
    TimerEvent_t event;
    event.eType_m = TIMER_EVENT_UNKNOWN;

    return startTimer(obj, nInterval, event);
}

bool SoftTimerMgr_c::startTimer(TimerInterface_i* obj, int nInterval, TimerEvent_t& event)
{
    TimerItem_t* pItem = NULL;

    // take item from pool if there is
    if (false == lstItemPool_m.isEmpty())
    {
        pItem = lstItemPool_m.takeLast();
    }

    if (NULL == pItem)
    {
        pItem = new TimerItem_t();
        if (NULL == pItem)
        {
            return false;
        }
    }

    unsigned int tCurrentTime  = System_c::getTimeTick() ;

    // append item to registerred list
    pItem->obj_m         = obj;
    pItem->nInterval_m   = nInterval;
    pItem->tStartTime_m  = tCurrentTime;
    pItem->event_m       = event;

    lstRegisteredItemList_m.append(pItem);

    checkTrigger(tCurrentTime, nInterval);

    //MSG("Add one timer, interval: %d, start time=%x\n", pItem->nInterval_m, pItem->tStartTime_m);

    return true;
}

void SoftTimerMgr_c::stopTimerAll(TimerInterface_i* obj)
{
    TimerHelperInfo info;
    info.pObj_m = obj;
    stopTimerHelper(&info, &SoftTimerMgr_c::stopTimerAllPtr);
}

void SoftTimerMgr_c::S_fire(void * object)
{
    SoftTimerMgr_c * psTimerMgr = (SoftTimerMgr_c*)object;
    psTimerMgr->fire();
}


void SoftTimerMgr_c::fire()
{
    const struct TimerOp_t * psTimerOp = get_x_timer_op();
    // stop timer
    psTimerOp->stop();

    // the timer will not be stopped, so start calculating next round fire up time
    long long currentTime;
    bool bFired = false;

    // check fire-up timer object
    currentTime     = System_c::getTimeTick() ;
    tStartTime_m    = currentTime;

    //MSG("Timer fired!!!, registered timer count: %d, current time=%x\n", lstRegisteredItemList_m.size(), currentTime);

    do
    {
        currentTime     = System_c::getTimeTick() ;
        bFired          = false;

        for (int nIndex=lstRegisteredItemList_m.size() - 1; nIndex>=0; nIndex--)
        {
            TimerItem_t* pItem = lstRegisteredItemList_m.at(nIndex);

            //MSG("Timer start time = %x\n", pItem->tStartTime_m);
            if ((NULL != pItem) && (pItem->nInterval_m <= ((currentTime - pItem->tStartTime_m ))))
            {
                //MSG("Process item with interval: %x\n", pItem->nInterval_m);

                lstRegisteredItemList_m.takeAt(nIndex);
                // append the item into backup pool
                lstItemPool_m.append(pItem);

                pItem->obj_m->timeout(pItem->event_m);

                bFired = true;
            }
        }

        //nRound++;
        // if some item fired, the processing interval may affect other items and makes them timeout too
        // so let's go back and take a look...
    } while (true == bFired);

    // since there may some new timer added during fire-up, so check left least elapse after 
    // completing timeout process

    int nClosestElapse  = 0x7fffffff;
    // To check how many timer left after fire up, new added timer in this period
    // will not be taken into account
    //int nOrigLeftTimer = 0;

    for (int nIndex=0; nIndex<lstRegisteredItemList_m.size(); nIndex++)
    {
        TimerItem_t* pItem = lstRegisteredItemList_m.at(nIndex);
        if (NULL != pItem)
        {
            int nLeftElapse = pItem->nInterval_m - (currentTime - pItem->tStartTime_m);
            if (nLeftElapse < nClosestElapse)
            {
                nClosestElapse   = nLeftElapse;
                //nOrigLeftTimer ++;
            }
        }
    }

    if(nClosestElapse < 0)
    {
        nClosestElapse = 0;
    }

    if (0 < lstRegisteredItemList_m.size())
    //if (nOrigLeftTimer > 0)
    {
        // start timer if there is object waiting
        psTimerOp->start(nClosestElapse);
        //pTimer_m->setInterval(nClosestElapse);
    }

    return;
}

struct TimerHelperEventInfo : TimerHelperInfo
{
    TimerEventType_e eEvent_m;
};

void SoftTimerMgr_c::stopTimerAllEvent(TimerInterface_i *obj, TimerEventType_e event)
{
    TimerHelperEventInfo info;
    info.pObj_m     = obj;
    info.eEvent_m   = event;

    stopTimerHelper(&info, &SoftTimerMgr_c::stopTimerAllEventPtr);
}

void SoftTimerMgr_c::stopTimerHelper(struct TimerHelperInfo * info, void (SoftTimerMgr_c::*func)(struct TimerHelperInfo *))
{
    for (int nIndex = lstRegisteredItemList_m.size() ; nIndex >= 0 ; --nIndex)
    {
        TimerItem_t* pItem = lstRegisteredItemList_m.at(nIndex);
        
        info->pItem_m = pItem;
        info->nIndex_m = nIndex;
        (this->*func)(info);
    }

    if (lstRegisteredItemList_m.size() <= 0)
    {
        const struct TimerOp_t * psTimerOp = get_x_timer_op();
        psTimerOp->stop();
    }

    return;
}

void SoftTimerMgr_c::stopTimerAllPtr(struct TimerHelperInfo * info)
{
    if ((NULL != info->pItem_m) && (info->pObj_m == info->pItem_m->obj_m))
    {
        lstRegisteredItemList_m.takeAt(info->nIndex_m);
        //std::cout << "remove one timer" << std::endl;

        // append the item into backup pool
        lstItemPool_m.append(info->pItem_m);
    }
}

void SoftTimerMgr_c::stopTimerAllEventPtr(struct TimerHelperInfo * pinfo)
{
    struct TimerHelperEventInfo * pEventInfo = (struct TimerHelperEventInfo  *)pinfo;

    if ((NULL != pEventInfo->pItem_m)
            && (pEventInfo->pObj_m == pEventInfo->pItem_m->obj_m)
            && (pEventInfo->eEvent_m == pEventInfo->pItem_m->event_m.eType_m))
    {
        lstRegisteredItemList_m.takeAt(pEventInfo->nIndex_m);
        //std::cout << "remove one timer" << std::endl;

        // append the item into backup pool
        lstItemPool_m.append(pEventInfo->pItem_m);
    }
}


