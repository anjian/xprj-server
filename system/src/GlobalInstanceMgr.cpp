#include <common/inc/MyStdLibrary.h>
#include <common/inc/MyDebug.h>

#include <interface/system/GlobalInstanceMgr.h>

static GlobalInstanceMgr_c* pInstance = NULL;

GlobalInstanceMgr_c::GlobalInstanceMgr_c() : 
    m_internal_store_count(0)
{
}

GlobalInstanceMgr_c::~GlobalInstanceMgr_c()
{
    // stack: release objects reversely
    for (int i=m_internal_store_count-1; i>=0; i--)
    {
        m_internal_store[i].m_destroy_func(m_internal_store[i].m_variable);
    }
}

GlobalInstanceMgr_c* GlobalInstanceMgr_c::getInstance()
{
    if (NULL == pInstance)
    {
        pInstance = new GlobalInstanceMgr_c();
    }

    return pInstance;
}

void GlobalInstanceMgr_c::destroy()
{
    if (NULL != pInstance)
    {
        delete pInstance;
        pInstance = NULL;
    }
}

void GlobalInstanceMgr_c::registerGlobalInstance(void * paVariable, void (*destroy_func)(void * aVariable))
{
    if (m_internal_store_count >= MAX_VARIABLE)
    {
        // this shit should never ever happend!
        // use valgrind or purifyplus to identify the error
        MSG_ERR("GLOBAL VARIABLE ERROR : count exceeding\n");
    }

    m_internal_store[m_internal_store_count].m_variable     = paVariable;
    m_internal_store[m_internal_store_count].m_destroy_func = destroy_func;
    m_internal_store_count++; 
}
