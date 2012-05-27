#ifndef __TIMER_OP_H_
#define __TIMER_OP_H_

//#include <interface/Interface.h>
//#include <common/inc/os_type.h>

typedef void (*x_timer_notify)(void * pData);

struct TimerOp_t
{
    bool (*create_timer)(void);
    void (*destroy)(void);

    void (*set_object_notify)(x_timer_notify pObjectNotify);
    void (*set_private_data)(void * pData);

    //void   (*set_interval)(int aInterval);
    void (*start)(int msec);
    void (*stop)(void);
    bool (*is_active)(void);
    int  (*get_interval)(void);
};

const struct TimerOp_t * get_x_timer_op();

#endif //TIMER_OP_H

