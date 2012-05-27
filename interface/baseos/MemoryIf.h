#ifndef __MEMORY_IF_H_
#define __MEMORY_IF_H_

#include <stddef.h>

#define MALLOC    xmalloc
#define FREE      xfree
#define REALLOC   xrealloc
#define MEMCHR    memchr

#ifdef HAVE_EXCEPTIONS
#ifdef __cplusplus
#include <exception>
#include <new>
#endif //__cplusplus
#define X_THROW(a) throw(a)
#define X_NO_THROW throw()
#else
#define X_THROW(a) 
#define X_NO_THROW
#endif //HAVE_EXCEPTIONS

#ifdef __cplusplus
extern "C" {
#endif 
    void* xmalloc(size_t size);
    void* xrealloc(void * ptr, size_t size, size_t oldSize);
    void xfree(void * ptr);

    // for the strdup helper
    char * xstrdup(const char * aChar);

#ifdef MEM_DBG
    void x_memdump();
    void x_mem_init(void);
#endif 

#ifdef __cplusplus
}

void * operator new (size_t size) X_THROW(std::bad_alloc);
void * operator new[] (size_t size) X_THROW(std::bad_alloc);

void operator delete (void * ptr) X_NO_THROW;
void operator delete[] (void * ptr) X_NO_THROW;
#endif 

#endif // __MEMORY_IF_H_
