#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>

#include <interface/baseos/MemoryIf.h>
#include <interface/system/GlobalInstanceMgr.h>
#include <interface/baseos/AppEventDispatchIf.h>

#include <baseos/inc/AppEventQueue.h>

#define APP_EVENT_QUEUE_DEFAULT_SIZE                100

#define APP_EVENT_QUEUE_ACTIVE_CHECK_INTERVAL       10
//#define APP_EVENT_QUEUE_IDLE_CHECK_INTERVAL         200

////////////////////////////////////////////////////////////////////////////////
// send event interface implementation
////////////////////////////////////////////////////////////////////////////////
bool APP_EVENT_QUEUE_START()
{
    AppEventQueue_c* pEventQueue = AppEventQueue_c::getInstance();
    if ((NULL == pEventQueue) || (false == pEventQueue->init()))
    {
        return false;
    }

    pEventQueue->start();

    return true;
}

void APP_EVENT_SENT(AppEvent_t& event)
{
    AppEventQueue_c::getInstance()->insertEvent(event);
}

void APP_EVENT_SET_DISPATCHER(AppEventDispatchIf_i* dispatcher)
{
    AppEventQueue_c::getInstance()->setDispatcher(dispatcher);
}

////////////////////////////////////////////////////////////////////////////////
// event queue
////////////////////////////////////////////////////////////////////////////////
//
AppEventQueue_c::AppEventQueue_c() :
    pDispatcher_m(NULL),
    nEventQueueSize_m(APP_EVENT_QUEUE_DEFAULT_SIZE),
    nEventQueueHeader_m(0),
    nEventQueueCount_m(0),
    pEventQueue_m(NULL)
{
}

AppEventQueue_c::~AppEventQueue_c()
{
    if (NULL != pEventQueue_m)
    {
        xfree(pEventQueue_m);
        pEventQueue_m = NULL;
    }

#if 0
    if(m_message_receiver)
    {
        IAppManager * appManager = system_engine_get_instance()->m_app_manager; 
        appManager->m_op->app_manager_destroy_receiver(appManager,m_message_receiver);
    }
#endif
}

IMPLE_OBJECT_GET_INSTANCE(AppEventQueue_c)

bool AppEventQueue_c::init()
{
#if 0
    // init event queue
    IAppManager * appManager = system_engine_get_instance()->m_app_manager; 
    m_message_receiver = appManager->m_op->app_manager_create_receiver(appManager,"ucweb");
    if(!m_message_receiver)
    {
        return UC_FALSE;
    }
    m_message_receiver->m_op->set_message_callback(m_message_receiver,MessageReceiverCallback,this);
    m_message_queue = system_engine_get_instance()->m_message_queue; 
#endif

    if (!allocateEventQueue(APP_EVENT_QUEUE_DEFAULT_SIZE))
    {
        return false;
    }

    return true;
}

void AppEventQueue_c::start()
{
    TIMER_START(this, APP_EVENT_QUEUE_ACTIVE_CHECK_INTERVAL);
}

bool AppEventQueue_c::allocateEventQueue(int nSize)
{
    AppEvent_t* pNewQueue = (AppEvent_t*)xmalloc(sizeof(AppEvent_t) * nSize);
    if (NULL == pNewQueue)
    {
        return false;
    }

    // copy events to new queue if there are
    if (NULL != pEventQueue_m)
    {
        for (int nIndex=0; nIndex<nEventQueueCount_m; nIndex++)
        {
            int nEventSlot = nEventQueueHeader_m + nIndex;
            if (nEventSlot >= nEventQueueSize_m)
            {
                nEventSlot -= nEventQueueSize_m;
            }

            pNewQueue[nIndex] = pEventQueue_m[nEventSlot];
        }

        xfree(pEventQueue_m);
    }

    MSG("Extend Web App Event Queue FROM(%d)==>To(%d)\n", nEventQueueSize_m, nSize);

    nEventQueueHeader_m = 0;
    nEventQueueSize_m   = nSize;
    pEventQueue_m       = pNewQueue;

    return true;
}


bool AppEventQueue_c::handleEvent()
{
#if 0
    UcTime tStartTime    = UcTime::getLocalTime();
    UcTime tCurrentTime;
#endif

    //MSG("check event queue...\n");

    bool bHandled    = false;

    AppEvent_t pWorkingEvent;
    while (retrieveEvent(pWorkingEvent))
    {
        // Don't lock the queue, since new event may be inserted during handling
        if (NULL != pDispatcher_m)
        {
            pDispatcher_m->dispatch(pWorkingEvent);
        }

#if 0
        tCurrentTime  = UcTime::getLocalTime();
        //UC_INT32 nInternval = tStartTime.msecsTo(tCurrentTime);
        //MSG("Consume one event in queue, left: %d, time used %d\n", nEventQueueCount_m, nInternval);
#endif

        bHandled = true;
        //postIfNeed();
    }
    
    return bHandled;
}

bool AppEventQueue_c::retrieveEvent(AppEvent_t& event)
{
    if (0 == nEventQueueCount_m)
    {
        //MSG("There is no message in the Queue!\n");
        return false;
    }

    // retrieve one event from queue, the oldest one
    event = pEventQueue_m[nEventQueueHeader_m];

    // adjust queue
    nEventQueueCount_m--;
    if (0 == nEventQueueCount_m)
    {
        // If message queue is empty, adjust header to the very first of the array
        nEventQueueHeader_m = 0;
    }
    else
    {
        nEventQueueHeader_m ++;
        if (nEventQueueHeader_m >= nEventQueueSize_m)
        {
            nEventQueueHeader_m = 0;
        }
    }


    return true;
}

/////////////////////////////////////////////////////
// Insert message into the queue
/////////////////////////////////////////////////////
bool AppEventQueue_c::insertEvent(AppEvent_t& event)
{

    // check if the same event has been in queue
    for (int nCount=0; nCount<nEventQueueCount_m; nCount++)
    {
        int nIndex = nCount + nEventQueueHeader_m;
        if (nIndex >= nEventQueueSize_m)
        {
            nIndex -= nEventQueueSize_m;
        }

        if (0 == xmemcmp(&pEventQueue_m[nIndex], &event, sizeof(AppEvent_t)))
        {
            //MSG("The same event has been in queue, return directly\n");

            return false;
        }
    }

    // if queue is full, re-allocate a bigger one
    if ((nEventQueueSize_m <= nEventQueueCount_m)
            && (false == allocateEventQueue(nEventQueueSize_m + APP_EVENT_QUEUE_DEFAULT_SIZE)))
    {
        MSG_ERR("Fail to extend App Event Queue from %d to %d\n", nEventQueueSize_m, nEventQueueSize_m + APP_EVENT_QUEUE_DEFAULT_SIZE);

        return false;
    }

    int nEventSlot = nEventQueueHeader_m + nEventQueueCount_m;
    if (nEventSlot >= nEventQueueSize_m)
    {
        nEventSlot -= nEventQueueSize_m;
    }

    pEventQueue_m[nEventSlot] = event;
    nEventQueueCount_m++;

#if 0
    //MSG("Insert item - slot: %d, total: %d\n", nEventSlot, nEventQueueCount_m);
    postIfNeed();
#endif

    return true;
}

void AppEventQueue_c::timeout(TimerEvent_t& /*event*/)
{
    handleEvent();

    TIMER_START(this, APP_EVENT_QUEUE_ACTIVE_CHECK_INTERVAL);
}


#if 0
void AppEventQueue_c::MessageReceiverCallback(void * aReceiverObject,UC_INT aMessageID,void * aSenderData)
{
    AppEventQueue_c * psEventQueue;

    psEventQueue = (AppEventQueue_c*)aReceiverObject;
    psEventQueue->m_bAlreadyPost = UC_FALSE;
    psEventQueue->handleEvent();
}


void AppEventQueue_c::handleAllPendingEvent()
{
    m_bAlreadyPost = UC_FALSE;
    while(nEventQueueCount_m > 0)
    {
        handleEvent();
    }
}

void AppEventQueue_c::postIfNeed()
{
    if(nEventQueueCount_m == 0 || m_bAlreadyPost)
    {
        return;
    }
    m_bAlreadyPost = UC_TRUE;
    IMESSAGEQUEUE_POST_MESSAGE(m_message_queue,m_message_receiver,0,NULL);
}
#endif
