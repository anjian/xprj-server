#ifndef __GLOBAL_INSTANCE_MGR_H_
#define __GLOBAL_INSTANCE_MGR_H_

class GlobalInstanceMgr_c
{
    public:
        ~GlobalInstanceMgr_c();

        static GlobalInstanceMgr_c* getInstance();
        static void destroy();

        void registerGlobalInstance(void * paVariable, void (*destroy_func)(void * aVariable));        

    protected:
        GlobalInstanceMgr_c();

    private:
        enum 
        {
            MAX_VARIABLE = 49
        };
        struct InternalEntry
        {
            void * m_variable;
            void (*m_destroy_func)(void * );
        };

        struct InternalEntry m_internal_store[MAX_VARIABLE];
        int m_internal_store_count;
};

#define IMPLE_OBJECT_GET_INSTANCE(type) \
static type * g_object = NULL; \
static void destroy(void * paObject) \
{ \
    type * psObject = (type*)(paObject); \
    delete psObject; \
} \
\
type* type::getInstance() \
{ \
    GlobalInstanceMgr_c * pGlobalInstanceMgr = GlobalInstanceMgr_c::getInstance(); \
    if (g_object == NULL) \
    { \
        g_object= new type(); \
        if(g_object) \
        { \
            pGlobalInstanceMgr->registerGlobalInstance(g_object, destroy); \
        } \
    } \
    \
    return g_object; \
}

#endif // UC_GLOBAL_VARIABLE_MGR_H_

