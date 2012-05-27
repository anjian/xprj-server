#ifndef __SIMPLE_VECTOR_H_
#define __SIMPLE_VECTOR_H_

#include <assert.h>
#include <interface/baseos/MemoryIf.h>
#include <interface/baseos/TransientMemory.h>

////////////////////////////////////////////////////////////////////////////////
// Very Simple dynamic array
//
//  Try to keep it as simple as possible, if it can not meet the requirement,
//  the recommended solution is to use other implmenetaion rather than add new
//  functionality
//
////////////////////////////////////////////////////////////////////////////////
template <class T> class SimpleVector
{
    public:

        inline SimpleVector (int initAlloc)
        {
            this->num = 0;
            this->numAlloc = initAlloc;
            void* temp = xmalloc (sizeof (T) * this->numAlloc);
            assert(temp);
            this->array = (T*)temp;
            //this->resize();
        }

        inline SimpleVector ()
        {
            this->num       = 0;
            this->numAlloc  = 0;
            this->array     = NULL;
        }


        /*
        inline SimpleVector (const SimpleVector &o) {
            this->array = NULL;
            this->num = o.num;
            this->numAlloc = o.numAlloc;
            resize ();
            memcpy (this->array, o.array, sizeof (T) * num);
        }
        */

        inline ~SimpleVector ()
        {
            if (this->array)
            {
                xfree (this->array);
            }
        }

        /**
         * \brief Return the number of elements put into this vector.
         */
        inline int size() const { return this->num; }

        inline T* getArray() { return array; }

        // push of stack
        inline void append(T t)
        {
            // if all items occupied, re-allocate the buffer
            if (this->num >= this->numAlloc)
            {
                setSize(this->num + 1);
            }
            else
            {
                this->num++;
            }

            this->array[this->num - 1] = t;
        }

        // remove item at given index
        inline T takeAt(int i)
        {
            assert (i >= 0 && this->num - i > 0);
            T ret = this->array[i];

            // if not the last item, need to move below items forward
            if ((this->num - 1) != i)
            {
                // xmemcpy overlap: to avoid 'undefined' risk, copy it ourselves
                //
                // NOTE: memmove's performance is not good, it'll copy the same content twice
                //xmemcpy(this->array + i, this->array + i + 1, sizeof(T) * (this->num - i - 1));
                char * pDes = (char*)(this->array + i);
                char * pSrc = (char*)(this->array + i + 1);
                int nLen = sizeof(T) * (this->num - i - 1);
                for (int nIndex=0; nIndex<nLen; nIndex++)
                {
                    pDes[nIndex] = pSrc[nIndex];
                }
            }

            this->num--;

            return ret;
        }


        // pop of stack
        inline T takeLast()
        {
            assert (this->num > 0);

            this->num--;
            return this->array[this->num];
        }

        // get last item in the list
        inline T getLast()
        {
            assert (this->num > 0);

            return this->array[this->num - 1];
        }
        ///////////////////////////////////////////////////////////////
        // NOTE: 
        //  it's not responsible for releasing object, and only reset 
        //  the number of item to zero.
        //
        //  Please release the object outside if needed
        ///////////////////////////////////////////////////////////////
        inline void clear()
        {
            this->num = 0;
        }

        ///**
        // * \brief Increase the vector size by one.
        // *
        // * May be necessary before calling misc::SimpleVector::set.
        // */
        //inline void increase() { setSize(this->num + 1); }

        /**
         * \brief Set the size explicitely.
         *
         * May be necessary before calling misc::SimpleVector::set.
         */
        inline void setSize(int newSize)
        {
            assert (newSize >= 0);
            this->num = newSize;
            this->resize ();
        }

        ///**
        // * \brief Set the size explicitely and initialize new values.
        // *
        // * May be necessary before calling misc::SimpleVector::set.
        // */
        //inline void setSize (int newSize, T t) {
        //    int oldSize = this->num;
        //    setSize (newSize);
        //    for (int i = oldSize; i < newSize; i++)
        //        set (i, t);
        //}

        /**
         * \brief Return the reference of one element.
         *
         * \sa misc::SimpleVector::get
         */
        inline T* getRef (int i) {
            assert (i >= 0 && this->num - i > 0);
            return array + i;
        }

        /**
         * \brief Return the one element, explicitety.
         *
         * The element is copied, so for complex elements, you should rather used
         * misc::SimpleVector::getRef.
         */
        inline T get (int i)
        {
            assert (i >= 0 && this->num - i > 0);
            return this->array[i];
        }

        inline const T get(int i) const
        {
            return ((SimpleVector<T>*)this)->get(i);
        }

        /**
         * \brief Store an object in the vector.
         *
         * Unlike in container::untyped::Vector and container::typed::Vector,
         * you have to care about the size, so a call to
         * misc::SimpleVector::increase or misc::SimpleVector::setSize may
         * be necessary before.
         */
        inline void set (int i, T t)
        {
            assert (i >= 0 && this->num - i > 0);
            this->array[i] = t;
        }

        void deleteAllObject()
        {
            int i;
            for(i = 0 ; i < num ; ++ i)
            {
                delete array[i];
            }
            num = 0 ;
        }

    protected:
        virtual void resize ()
        {
            /* This algorithm was tunned for memory&speed with this huge page:
             *   http://downloads.mysql.com/docs/refman-6.0-en.html.tar.gz
             */
            if (array == NULL)
            {
                this->numAlloc = 1;
                this->array = (T*) xmalloc (sizeof (T));
            }

            if (this->numAlloc < this->num)
            {
                int sOldNumAlloc = this->numAlloc;
                this->numAlloc = (this->num < 100) ?  this->num : this->num + this->num/10;
                this->array = (T*) xrealloc(this->array, (this->numAlloc * sizeof (T)),(sOldNumAlloc * sizeof (T) ) );
            }
            assert(NULL != this->array);
        }

    protected:
        T *array;
        int num;
        int numAlloc;

};

////////////////////////////////////////////////////////////////////////////////
// Very Simple temp dynamic array: All memory allocated from transient pool
//
//  Try to keep it as simple as possible, if it can not meet the requirement,
//  the recommended solution is to use other implmenetaion rather than add new
//  functionality
//
////////////////////////////////////////////////////////////////////////////////
template <class T> class TempSimpleVector_c : public SimpleVector<T>
{
    public:

        inline TempSimpleVector_c(int initAlloc)// : SimpleVector<T>(initAlloc)
        {
            this->num       = 0;
            this->numAlloc  = initAlloc;
            void* temp = TR_MEMORY_ALLOC(sizeof (T) * this->numAlloc);
            assert(temp);
            this->array = (T*)temp;
        }

        inline ~TempSimpleVector_c ()
        {
            if (this->array)
            {
                TR_MEMORY_FREE(this->array);
                this->array = NULL;
            }

            this->num       = 0;
            this->numAlloc  = 0;
        }

#if 0
        void* operator new (size_t size)   // overload to allocate from transient pool
        {
            return TR_MEMORY_ALLOC(size);
        }

        void operator delete (void *p)     // cooresponding to new
        {
            TR_MEMORY_FREE(p);
        }
#endif


#if 0
        /**
         * \brief Return the number of elements put into this vector.
         */
        inline int size() { return this->num; }

        inline T* getArray() { return array; }

        // push of stack
        inline void append(T t)
        {
            // if all items occupied, re-allocate the buffer
            if (this->num >= this->numAlloc)
            {
                setSize(this->num + 1);
            }
            else
            {
                this->num++;
            }

            this->array[this->num - 1] = t;
        }

        // pop of stack
        inline T takeLast()
        {
            assert (this->num > 0);

            this->num--;
            return this->array[this->num];
        }

        ///////////////////////////////////////////////////////////////
        // NOTE: 
        //  it's not responsible for releasing object, and only reset 
        //  the number of item to zero.
        //
        //  Please release the object outside if needed
        ///////////////////////////////////////////////////////////////
        inline void clear()
        {
            this->num = 0;
        }

        ///**
        // * \brief Increase the vector size by one.
        // *
        // * May be necessary before calling misc::SimpleVector::set.
        // */
        //inline void increase() { setSize(this->num + 1); }

        /**
         * \brief Set the size explicitely.
         *
         * May be necessary before calling misc::SimpleVector::set.
         */
        inline void setSize(int newSize)
        {
            assert (newSize >= 0);
            this->num = newSize;
            this->resize ();
        }

        ///**
        // * \brief Set the size explicitely and initialize new values.
        // *
        // * May be necessary before calling misc::SimpleVector::set.
        // */
        //inline void setSize (int newSize, T t) {
        //    int oldSize = this->num;
        //    setSize (newSize);
        //    for (int i = oldSize; i < newSize; i++)
        //        set (i, t);
        //}

        /**
         * \brief Return the reference of one element.
         *
         * \sa misc::SimpleVector::get
         */
        inline T* getRef (int i) {
            assert (i >= 0 && this->num - i > 0);
            return array + i;
        }

        /**
         * \brief Return the one element, explicitety.
         *
         * The element is copied, so for complex elements, you should rather used
         * misc::SimpleVector::getRef.
         */
        inline T get (int i)
        {
            assert (i >= 0 && this->num - i > 0);
            return this->array[i];
        }

        /**
         * \brief Store an object in the vector.
         *
         * Unlike in container::untyped::Vector and container::typed::Vector,
         * you have to care about the size, so a call to
         * misc::SimpleVector::increase or misc::SimpleVector::setSize may
         * be necessary before.
         */
        inline void set (int i, T t)
        {
            assert (i >= 0 && this->num - i > 0);
            this->array[i] = t;
        }

    private:
        T *array;
        int num, numAlloc;
#endif

    protected:
        void resize ()
        {
            /* This algorithm was tunned for memory&speed with this huge page:
             *   http://downloads.mysql.com/docs/refman-6.0-en.html.tar.gz
             */
            if (this->array == NULL)
            {
                int nSize   = sizeof(T);
                this->numAlloc = 1;
                void* temp  = TR_MEMORY_ALLOC(nSize * this->numAlloc);
                this->array = (T*) temp;
            }

            if (this->numAlloc < this->num)
            {
                int nOldSize    = this->numAlloc * sizeof(T);

                this->numAlloc  = (this->num < 100) ?  this->num : this->num + this->num/10;
                this->array     = (T*)TR_MEMORY_REALLOC(this->array, nOldSize, (this->numAlloc * sizeof (T)));
            }
        }

};

#endif
