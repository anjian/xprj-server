#ifndef __SYSTEM_H__
#define __SYSTEM_H__

class System_c
{
    public:
        static void debug_print(const char * ,...);
        static void exit(int nCode);

        static long long getTimeTick();
};

#endif // __SYSTEM_H__
