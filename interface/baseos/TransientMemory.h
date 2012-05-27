#ifndef __TRANSIENT_MEMORY_H_
#define __TRANSIENT_MEMORY_H_

#ifdef __cplusplus 

#include <stddef.h>

extern "C" {
#endif

    void* TR_MEMORY_ALLOC(size_t size);
    void* TR_MEMORY_REALLOC(void* ptr, size_t nOrigSize, size_t nNewSize);
    void  TR_MEMORY_FREE(void* ptr);

#ifdef __cplusplus 
}
#endif

#define SAFE_DELETE_TR_PTR(ptr) \
    if(NULL != ptr)             \
    {                           \
        TR_MEMORY_FREE(ptr);    \
        ptr = NULL;             \
    }

#endif

