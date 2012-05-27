#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <common/inc/MyDebug.h>

#include <interface/platform/TimerOp.h>

#include <platform/inc/xtimer.h>

//#define SIGTIMER     (SIGRTMAX)
#define ONESHOTTIMER (SIGRTMAX-1)

//timer_t SetTimer(int signo, int sec, int mode);
void xtimer_signalHandler(int signo, siginfo_t * info, void *context);

static bool bTimerInited_g      = false;
static timer_t tOneShotTimer_g  = (timer_t) - 1;
 
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
static x_timer_notify g_notify;
static void * g_notify_object;

static void set_object_notify(x_timer_notify pObjectNotify)
{
    g_notify = pObjectNotify;
}

static void set_private_data(void * pData)
{
    g_notify_object = pData;
}

void xtimer_signalHandler(int signo, siginfo_t * /*info*/, void * /*context*/)
{
    if ((NULL == g_notify_object) || (NULL == g_notify))
    {
        return;
    }

#if 0
    if (signo == SIGTIMER)
    {
        puts("Periodic timer");
    }
#endif

    if (ONESHOTTIMER == signo)
    {
        //MSG("One-short timer\n");

        g_notify(g_notify_object);
    }

#if 0
    else if (signo == SIGINT) {
        //timer_delete(oneshotTimer);
        timer_delete(timerid);
        perror("Ctrl + C cached!\n");
        exit(1);
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////
bool xtimer_init()
{
    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags     = SA_SIGINFO;
    sigact.sa_sigaction = xtimer_signalHandler;

    // Set up sigaction to catch signal
    //if (sigaction(SIGTIMER, &sigact, NULL) == -1)
    if (sigaction(ONESHOTTIMER, &sigact, NULL) == -1)
    {
        //perror("sigaction failed");
        return false;
    }

    // create timer
    {
        struct sigevent sigev;

        sigev.sigev_notify  = SIGEV_SIGNAL;
        sigev.sigev_signo   = ONESHOTTIMER;
        sigev.sigev_value.sival_ptr = &tOneShotTimer_g;

        if (timer_create(CLOCK_REALTIME, &sigev, &tOneShotTimer_g) < 0)
        {
            return false;
        }
    }
#if 0
    // Establish a handler to catch CTRL+C and use it for exiting
    sigaction(SIGINT, &sigact, NULL);

    timerid         = SetTimer(SIGTIMER, 1000, 1);
    oneshotTimer = SetTimer(ONESHOTTIMER, 1000, 0);
    for(;;)
        ;
#endif

    bTimerInited_g = true;

    return true;
}

bool xtimer_start(int msec)
{
    if (!bTimerInited_g)
    {
        return false;
    }

    struct itimerspec itval;
    struct itimerspec oitval;

    itval.it_value.tv_sec       = msec / 1000;
    itval.it_value.tv_nsec      = (long)(msec % 1000) * (1000000L);

    // only support one shot timer
    itval.it_interval.tv_sec    = 0;
    itval.it_interval.tv_nsec   = 0;

    if (timer_settime(tOneShotTimer_g, 0, &itval, &oitval) < 0)
    {
        //perror("time_settime error!");
        return false;
    }

    return true;
}

bool xtimer_stop()
{
    if (!bTimerInited_g)
    {
        return false;
    }

    struct itimerspec itval;
    struct itimerspec oitval;

    // set expiration to 0, stop the timer
    itval.it_value.tv_sec       = 0;
    itval.it_value.tv_nsec      = 0;

    itval.it_interval.tv_sec    = 0;
    itval.it_interval.tv_nsec   = 0;

    if (timer_settime(tOneShotTimer_g, 0, &itval, &oitval) < 0)
    {
        //perror("time_settime error!");
        return false;
    }

    return true;
}

bool xtimer_isActive()
{
    if (!bTimerInited_g)
    {
        return false;
    }

    struct itimerspec curitval;

    if (timer_gettime(tOneShotTimer_g, &curitval) < 0)
    {
        return false;
    }

    return ((0 != curitval.it_value.tv_sec) || (0L != curitval.it_value.tv_nsec));
}

int xtimer_getInterval()
{
    if (!bTimerInited_g)
    {
        return 0;
    }

    struct itimerspec curitval;

    if (timer_gettime(tOneShotTimer_g, &curitval) < 0)
    {
        return 0;
    }

    return curitval.it_value.tv_sec * 1000 + curitval.it_value.tv_nsec / 1000000L;
}

void xtimer_destroy()
{
    if (!bTimerInited_g)
    {
        return;
    }

    timer_delete(tOneShotTimer_g);
    bTimerInited_g = false;
}

////////////////////////////////////////////////////////////////////////////////
// external interface
////////////////////////////////////////////////////////////////////////////////
//
static const struct TimerOp_t g_op = {
    /* create_timer = */                    xtimer_init,
    /* destroy = */                         xtimer_destroy,
    /* set_object_notify = */               set_object_notify,
    /* set_private_data = */                set_private_data,
    /* start = */                           xtimer_start,
    /* is_active = */                       xtimer_isActive,
    /* get_interval = */                    xtimer_getInterval,
    /* stop = */                            xtimer_stop
};

const struct TimerOp_t * get_x_timer_op()
{
    return &g_op;
}

#if 0
#include <unistd.h>

int main()
{
    xtimer_init();

    MSG("Start timer\n");
    xtimer_start(500);

    MSG("Sleep one second\n");
    sleep(1);
    MSG("left timer : %d\n", xtimer_getInterval());
    if (xtimer_isActive())
    {
        xtimer_stop();
        MSG("Stop timer\n");
    }

    for (;;) {}

    xtimer_destroy();

    return 0;
}
#endif
