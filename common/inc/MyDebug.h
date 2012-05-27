#ifndef __DEBUG_H_
#define __DEBUG_H_

/*
 * You can disable any MSG* macro by adding the '_' prefix.
 */
#ifdef NO_LOG_AT_ALL

#define MSG(...)
#define MSG_WARN(...)
#define MSG_ERR(...)

#define MSG_TRMEM(...)
#define MSG_TRMEM_WARN(...)
#define MSG_TRMEM_ERR(...)

#define MSG_IO(...)
#define MSG_IO_WARN(...)
#define MSG_IO_ERR(...)

#else 

#define prefs_show_msg      1

#define STMT_START          do
#define STMT_END            while (0)

#include <common/inc/MyTime.h>
#include <common/inc/MySystem.h>

#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
// assert
////////////////////////////////////////////////////////////////////////////////
#define DBG_ASSERT(exp)     \
    STMT_START {         \
        assert(exp);        \
    } STMT_END

////////////////////////////////////////////////////////////////////////////////
// helper functions
////////////////////////////////////////////////////////////////////////////////
#define MSG_TIME_STAMP                                                      \
{                                                                           \
    Time_c rawtime = Time_c::getLocalTime();                                \
    System_c::debug_print("%s(L:%d)",__FILE__,__LINE__);                    \
    System_c::debug_print("%04d/%02d/%02d %02d:%02d:%02d.%03u ",            \
            (int)rawtime.getYears(),                                        \
            (int)rawtime.getMonths(),                                       \
            (int)rawtime.getDays(),                                         \
            (int)rawtime.getHours(),                                        \
            (int)rawtime.getMinutes(),                                      \
            (int)rawtime.getSec(),                                          \
            (unsigned int)rawtime.getMillsec());                            \
}

////////////////////////////////////////////////////////////////////////////////
// common
////////////////////////////////////////////////////////////////////////////////
#define MSG(...)                                                            \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print(__VA_ARGS__);                             \
        }                                                                   \
    } STMT_END

#define MSG_WARN(...)                                                       \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("** WARNING **: " __VA_ARGS__);           \
        }                                                                   \
    } STMT_END

#define MSG_ERR(...)                                                        \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("** ERROR **: " __VA_ARGS__);             \
        }                                                                   \
    } STMT_END

////////////////////////////////////////////////////////////////////////////////
// transiment memory debug marco
////////////////////////////////////////////////////////////////////////////////
#define MSG_TRMEM(...)
#define MSG_TRMEM_WARN(...)
#define MSG_TRMEM_ERR(...)

#define _MSG_TRMEM(...)                                                     \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("[TRMEM] : "__VA_ARGS__);                 \
        }                                                                   \
    } STMT_END

#define _MSG_TRMEM_WARN(...)                                                \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("[TRMEM] ** WARNING **: " __VA_ARGS__);   \
        }                                                                   \
    } STMT_END

#define _MSG_TRMEM_ERR(...)                                                 \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("[TRMEM] ** ERROR **: " __VA_ARGS__);     \
        }                                                                   \
    } STMT_END

////////////////////////////////////////////////////////////////////////////////
// IO debug marco
////////////////////////////////////////////////////////////////////////////////
#define _MSG_IO(...)
#define _MSG_IO_WARN(...)
#define _MSG_IO_ERR(...)

#define MSG_IO(...)                                                         \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("[IO] : "__VA_ARGS__);                    \
        }                                                                   \
    } STMT_END

#define MSG_IO_WARN(...)                                                    \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("[IO] ** WARNING **: " __VA_ARGS__);      \
        }                                                                   \
    } STMT_END

#define MSG_IO_ERR(...)                                                     \
    STMT_START {                                                         \
        if (prefs_show_msg)                                                 \
        {                                                                   \
            MSG_TIME_STAMP                                                  \
            System_c::debug_print("[IO] ** ERROR **: " __VA_ARGS__);        \
        }                                                                   \
    } STMT_END

#endif // NO_LOG_AT_ALL

#endif // __DEBUG_H_ 
