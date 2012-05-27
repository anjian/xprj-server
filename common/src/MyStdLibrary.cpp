//#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//#include <interface/SystemEngine.h>
//#include <interface/StdLibraryIface.h>

#include <interface/baseos/MemoryIf.h>

#include <common/inc/MyCType.h>
#include <common/inc/MyStdLibrary.h>

//static struct StdLibraryIface const * get_std_library_iface()
//{
//    return system_engine_get_instance()->m_std_library_iface;
//}

size_t xstrlen(const char *s)
{
    return strlen(s);
}

int xstrcmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

int xstrncmp(const char * s1, const char * s2, size_t size)
{
    return strncmp(s1, s2, size);
}

int xstricmp(const char *s1, const char *s2)
{
    return strcasecmp(s1, s2);
}

int xstrnicmp(const char * s1, const char * s2, size_t size)
{
    return strncasecmp(s1, s2, size);
}

int xmemcmp(const void *s1, const void *s2, size_t n)
{
    return memcmp(s1, s2, n);
}

void * xmemcpy(void *_RESTRICT_QUANLIFIER s1, const void * _RESTRICT_QUANLIFIER s2, size_t size)
{
    return memcpy(s1, s2, size);
}

void * xmemmove(void *s1, const void *s2, size_t size)
{
    return memmove(s1, s2, size);
}

void * xmemset(void *b, int c, size_t size)
{
    return memset(b, c, size);
}

int xstrtol(const char *_RESTRICT_QUANLIFIER str, char **_RESTRICT_QUANLIFIER endptr, int base)
{
    return strtol(str, endptr, base);  
}

int xatoi(const char * str)
{
    return atoi(str);
}

int xabs(int num)
{
    return abs(num);
}

char * xstrcpy(char *_RESTRICT_QUANLIFIER s1, const char *_RESTRICT_QUANLIFIER s2)
{
    return strcpy(s1, s2);
}

char * xstrcat(char *_RESTRICT_QUANLIFIER s1, const char *_RESTRICT_QUANLIFIER s2)
{
    return strcat(s1, s2);
}

double   xstrtod(const char *_RESTRICT_QUANLIFIER nptr, char **_RESTRICT_QUANLIFIER endptr )
{
    return strtod(nptr ,endptr);
}

char * xstrchr(const char * str, int character )
{
    return strchr((char*)str, character);
}

char * xstrrchr(const char * str, int character)
{
    return strrchr((char*)str, character);
}

const char * xstrstr(const char * s1,const char * s2)
{
    return strstr(s1, s2);
}

char * xstrpbrk(const char *s, const char *accept)
{
    const char * sRet = s;
    for(;*sRet; ++sRet)
    {
        for(const char * i = accept; *i ; ++i)
        {
            if(*i == *sRet)
            {
                return (char*)sRet;
            }
        }
    }

    return NULL;
}

/**
 * g_strdup:
 * @str: the string to duplicate
 *
 * Duplicates a string. If @str is %NULL it returns %NULL.
 * The returned string should be freed with g_free()
 * when no longer needed.
 *
 * Returns: a newly-allocated copy of @str
 */
char* xstrdup(const char *str)
{
    char *new_str;
    size_t length;

    if (str)
    {
        length = xstrlen (str) + 1;
        new_str = (char*)xmalloc(length);
        if (NULL != new_str)
        {
            xmemcpy(new_str, str, length);
            new_str[length] = 0;
        }
    }
    else
    {
        new_str = NULL;
    }

    return new_str;
}


// int
// xtolower (int c)
// {
//     if( c >= 'A' && c <= 'Z')
//     {
//         return (int)'a' + c - 'A';
//     }
//     return c;
// }
// 
// bool 
// xisdigit ( int c)
// {
//     return (c >= '0' && c <= '9');
// }
// 
// bool
// xisalpha ( int c)
// {
//     return ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) ;
// }
// 
// 
// bool
// xisalnum (int c)
// {
//     return xisalpha(c) || xisdigit(c);
// }
// 
// 
// bool
// xisxdigit (int c)
// {
//     return (c >= '0' && c <= '9') || ( c >= 'a' && c <= 'f') || ( c >= 'A' && c <= 'F');
// }
// 
// 
// bool
// xisupper (int c)
// {
//     return ( c >= 'A' && c <= 'Z');
// }
