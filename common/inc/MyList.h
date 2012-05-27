#ifndef __LIST_H_
#define __LIST_H_

#include <common/inc/glist.h>

// warning : This class support only pointers
template <class T> class List_c
{
    public:
        List_c() : 
            m_list(NULL),
            m_size(0U)
        {
        }

        ~List_c()
        {
            clear();
        }

    public:
        // append a pointer to specify object to the list
        bool append(T pObject)
        {
            GList * sNewHead = g_list_append(m_list,(gpointer)pObject);
            if(sNewHead)
            {
                m_list = sNewHead;
                m_size++;
                return true;
            }
            return false;
        }

        T takeAt(int nIndex)
        {
            if(nIndex >= m_size)
            {
                return NULL;
            }
            GList * pslink = g_list_nth(m_list, nIndex); 
            if(!pslink)
            {
                return NULL;
            }
            T sData = (T)(unsigned long)pslink->data;
            m_list = g_list_delete_link(m_list,pslink);
            m_size--;
            return sData;
        }

        T takeLast()
        {
            return takeAt(m_size - 1);
        }

        T at(int nIndex)
        {
            if(nIndex >= m_size)
            {
                return (T)(NULL);
            }
            return (T)(unsigned long)g_list_nth_data(m_list, nIndex);
        }

        void replace (int nIndex, T aData)
        {
            GList * psList = g_list_nth(m_list, nIndex);
            if(psList)
            {
                psList->data = aData;
            }
        }

        int size() const
        {
            return m_size;
        }

        bool insert(int nIndex, T aData)
        {
            GList * sNewHead = g_list_insert(m_list, (gpointer)aData, nIndex);
            if(sNewHead)
            {
                m_list = sNewHead;
                m_size++;
                return true;
            }
            return false;
        }

        bool isEmpty() const
        {
            return m_size == 0;
        }

        void clear()
        {
            if(m_list)
            {
                g_list_free(m_list);
                m_list = NULL;
                m_size = 0 ;
            }
        }

        T last()
        {
            return (T)(unsigned long)g_list_nth_data(m_list, m_size - 1);
        }

    public:
        typedef T type;
        GList * m_list; // do not use it directly , it's public just because ads is a stupid compiler
        int     m_size;
};

#define LIST_CLEAR_AND_DELETE_CONTENT(l, type) \
    { \
        GList * sList = (l)->m_list; \
        while(sList)\
        {\
            GList * sNList = sList->next;\
            delete ((type)sList->data);\
            xfree(sList);\
            sList = sNList;\
        }\
        l->m_list = NULL;\
        l->m_size = 0;\
    }

#endif //__LIST_H_

