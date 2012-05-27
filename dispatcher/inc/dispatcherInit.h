#ifndef __DISPATCHER_INIT_H_
#define __DISPATCHER_INIT_H_

class DispatcherInit_c
{
    public:
        ~DispatcherInit_c();
        static DispatcherInit_c* getInstance();

        bool initialize();

    protected:
        DispatcherInit_c();

    private:
};

#endif
