#ifndef __MY_HASH_H_
#define __MY_HASH_H_

#include <interface/baseos/MemoryIf.h>

#include <common/inc/ghash.h>

enum HashKeyType_e
{
    HASH_KEY_STRING,
    HASH_KEY_INT,
};

template <class T> class Hash_c
{
    public:
        Hash_c() :
            htTable_m(NULL),
            bIteratorInited_m(false)
        {
        }


        virtual ~Hash_c()
        {
            if (NULL != htTable_m)
            {
                g_hash_table_destroy(htTable_m);
                htTable_m = NULL;
            }
        }

        void remove_all()
        {
            if (NULL == htTable_m)
            {
                return;
            }

            g_hash_table_remove_all(htTable_m);
        }


#if 0
        ////////////////////////////////////////////////////////////////////////////////
        // int type key operation
        ////////////////////////////////////////////////////////////////////////////////
        void insert(long long* key, T value)
        {
            if ((NULL == htTable_m) || (HASH_KEY_INT != eKeyType_m))
            {
                return;
            }

            g_hash_table_insert(htTable_m, key, value);
        }

        void replace(long long* key, T value)
        {
            if ((NULL == htTable_m) || (HASH_KEY_INT != eKeyType_m))
            {
                return;
            }

            g_hash_table_replace(htTable_m, key, value);
        }

        T remove(long long key)
        {
            if ((NULL == htTable_m) || (HASH_KEY_INT != eKeyType_m))
            {
                return;
            }

            T ret = (T) g_hash_table_lookup(htTable_m, &key);

            g_hash_table_remove(htTable_m, &key);

            return ret;
        }

        T lookup(long long key)
        {
            if ((NULL == htTable_m) || (HASH_KEY_INT != eKeyType_m))
            {
                return (T)NULL;
            }

            return (T) g_hash_table_lookup(htTable_m, &key);
        }
#endif

        ////////////////////////////////////////////////////////////////////////////////
        // iterator functions
        ////////////////////////////////////////////////////////////////////////////////
        void initIterator()
        {
            if (NULL == htTable_m)
            {
                return;
            }

            bIteratorInited_m = true;
            g_hash_table_iter_init(&iter_m, htTable_m);
        }

        bool next(void* key, T* value)
        {
            if (!bIteratorInited_m)
            {
                return false;
            }

            return g_hash_table_iter_next(htTable_m, key, value);
        }

        void clearIterator()
        {
            bIteratorInited_m = false;
        }

    protected:
        GHashTable* htTable_m;

    private:
        bool bIteratorInited_m;

        GHashTableIter iter_m;
};

template <class T> class HashString_c : public Hash_c<T> 
{
    public:
        HashString_c()
        {
            this->htTable_m = g_hash_table_new_full(g_str_hash,
                    g_str_equal,
                    (GDestroyNotify)xfree,
                    NULL);
        }

        void insert(const char* key, T value)
        {
            if (NULL == this->htTable_m)
            {
                return;
            }

            // duplicate key
            char* sKey  = xstrdup(key);
            if (NULL == sKey)
            {
                return;
            }

            g_hash_table_insert(this->htTable_m, sKey, value);
        }
        

        void replace(const char* key, T value)
        {
            if (NULL == this->htTable_m)
            {
                return;
            }

            // duplicate key
            char* sKey  = xstrdup(key);
            if (NULL == sKey)
            {
                return;
            }

            g_hash_table_replace(this->htTable_m, sKey, value);
        }

        T remove(char* key)
        {
            if (NULL == this->htTable_m)
            {
                return NULL;
            }

            T ret = (T) g_hash_table_lookup(this->htTable_m, key);

            g_hash_table_remove(this->htTable_m, key);

            return ret;
        }

        T lookup(char* key)
        {
            if (NULL == this->htTable_m)
            {
                return (T)NULL;
            }

            return (T) g_hash_table_lookup(this->htTable_m, key);
        }

    protected:
    private:
};


template <class T> class HashInt_c : public Hash_c<T> 
{
    public:
        HashInt_c()
        {
            this->htTable_m = g_hash_table_new_full(g_int_hash,
                    g_int_equal,
                    NULL,
                    NULL);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // int type key operation
        ////////////////////////////////////////////////////////////////////////////////
        void insert(int* key, T value)
        {
            if (NULL == this->htTable_m)
            {
                return;
            }

            g_hash_table_insert(this->htTable_m, key, value);
        }

        void replace(int* key, T value)
        {
            if (NULL == this->htTable_m)
            {
                return;
            }

            g_hash_table_replace(this->htTable_m, key, value);
        }

        T remove(int key)
        {
            if (NULL == this->htTable_m)
            {
                return NULL;
            }

            T ret = (T) g_hash_table_lookup(this->htTable_m, &key);

            if (NULL != ret)
            {
                g_hash_table_remove(this->htTable_m, &key);
            }

            return ret;
        }

        T lookup(int key)
        {
            if (NULL == this->htTable_m)
            {
                return (T)NULL;
            }

            return (T) g_hash_table_lookup(this->htTable_m, &key);
        }

    protected:
    private:
};

template <class T> class HashLong_c : public Hash_c<T> 
{
    public:
        HashLong_c()
        {
            this->htTable_m = g_hash_table_new_full(g_int64_hash,
                    g_int64_equal,
                    NULL,
                    NULL);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // int type key operation
        ////////////////////////////////////////////////////////////////////////////////
        void insert(long long* key, T value)
        {
            if (NULL == this->htTable_m)
            {
                return;
            }

            g_hash_table_insert(this->htTable_m, key, value);
        }

        void replace(long long* key, T value)
        {
            if (NULL == this->htTable_m)
            {
                return;
            }

            g_hash_table_replace(this->htTable_m, key, value);
        }

        T remove(long long key)
        {
            if (NULL == this->htTable_m)
            {
                return NULL;
            }

            T ret = (T) g_hash_table_lookup(this->htTable_m, &key);

            if (NULL != ret)
            {
                g_hash_table_remove(this->htTable_m, &key);
            }

            return ret;
        }

        T lookup(long long key)
        {
            if (NULL == this->htTable_m)
            {
                return (T)NULL;
            }

            return (T) g_hash_table_lookup(this->htTable_m, &key);
        }

    protected:
    private:
};

#endif
