#ifndef __MY_STD_LIBRARY_H_
#define __MY_STD_LIBRARY_H_

#include <stdarg.h>

#include <common/inc/os_type.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

    const char * xstrstr(const char * s1, const char * s2);
    size_t xstrlen(const char *s);
    int xstrcmp(const char *s1, const char *s2);
    int xstrncmp(const char * s1, const char * s2, size_t aSize);
    int xstricmp(const char *s1, const char *s2);
    int xstrnicmp(const char * s1, const char * s2, size_t aSize);

    int xsnprintf(char *_RESTRICT_QUANLIFIER paBuf, size_t aSize, const char *_RESTRICT_QUANLIFIER paFormat,...);
    int xsprintf(char * _RESTRICT_QUANLIFIER s, const char * _RESTRICT_QUANLIFIER format, ...);
    int xvsnprintf(char *_RESTRICT_QUANLIFIER s, size_t n, const char *_RESTRICT_QUANLIFIER format, va_list ap);

    int xmemcmp(const void *s1, const void *s2, size_t n);

    void * xmemcpy(void *_RESTRICT_QUANLIFIER s1, const void * _RESTRICT_QUANLIFIER s2, size_t n);
    void * xmemmove(void *s1, const void *s2, size_t n);
    void * xmemset(void *b, int c, size_t len);

    int xstrtol(const char *_RESTRICT_QUANLIFIER str, char **_RESTRICT_QUANLIFIER endptr, int base);
    int xatoi(const char * str);
    int xabs(int aNum);

    char * xstrcpy(char *_RESTRICT_QUANLIFIER s1, const char *_RESTRICT_QUANLIFIER s2);
    char * xstrcat(char *_RESTRICT_QUANLIFIER s1, const char *_RESTRICT_QUANLIFIER s2);
    double xstrtod(const char *_RESTRICT_QUANLIFIER nptr, char **_RESTRICT_QUANLIFIER endptr);
    char * xstrchr(const char * str, int character);
    char * xstrrchr(const char * str, int character);
    char * xstrpbrk(const char *s, const char *accept);

    char* xstrdup(const char *str);
//#include <com/inc/uc_ctype.h>


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__MY_STD_LIBRARY_H_

