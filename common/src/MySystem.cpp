//#include <com/inc/UcStdLibrary.h>
//#include <interface/SystemOp.h>
//#include <interface/SystemEngine.h>
//#include <interface/uc_memory.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <common/inc/MySystem.h>

void System_c::debug_print( const char * sFormat,... )
{
    //const struct SystemEngine * psSystemEngine = system_engine_get_instance();
    //const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;
    char sBuf[256];

    va_list sList;
    va_start(sList, sFormat);
    vsnprintf(sBuf,255, sFormat, sList);
    sBuf[255] = 0;
    va_end(sList);

    printf("%s", sBuf);

    //psSystemOp->debug_print(sBuf);
}

void System_c::exit(int nCode)
{
    //const struct SystemEngine * psSystemEngine = system_engine_get_instance();
    //const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;

    //psSystemOp->exit(aCode,UC_FALSE);
    exit(nCode);
}

long long System_c::getTimeTick()
{
//    const struct SystemEngine * psSystemEngine = system_engine_get_instance();
//    const struct SystemOp * psSystemOp = psSystemEngine->m_system_op;
//
//    return psSystemOp->get_tick_count();
    //get the current number of microseconds since the system was startup
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now))
    {
        return 0L;
    }

    return now.tv_sec * 1000.0L + now.tv_nsec / 1000000.0L;
}
