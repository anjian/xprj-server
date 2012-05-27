//#define MEM_DBG 1
//
//#include <interface/SystemEngine.h>
//#include <interface/StdLibraryIface.h>

#include <stdlib.h>

#include <interface/baseos/MemoryIf.h>

void * xmalloc(size_t size)
{
    return malloc(size);
    //return system_engine_get_instance()->m_std_library_iface->malloc(aSize);
}

void xfree(void * ptr)
{
    free(ptr);
    //system_engine_get_instance()->m_std_library_iface->free(aPtr);
}

void * xrealloc(void* ptr, size_t size,size_t /*oldSize*/)
{
    return realloc(ptr, size);
    //return system_engine_get_instance()->m_std_library_iface->realloc(aPtr,aSize,aOldSize);
}

void * operator new (size_t size) X_THROW(std::bad_alloc)
{
    return xmalloc(size);
}

void * operator new[] (size_t size) X_THROW(std::bad_alloc)
{
    return xmalloc(size);
}

void operator delete (void * ptr) X_NO_THROW
{
    xfree(ptr);
}

void operator delete[] (void * ptr)  X_NO_THROW
{
    xfree(ptr);
}
