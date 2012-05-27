#ifndef __TLV_ATTR_SET_H_
#define __TLV_ATTR_SET_H_

#include <common/inc/SimpleVector.h>

#include <interface/protocol/TlvIf.h>

class TlvAttrSet_c : public TlvAttrIf_i
{
    public:
        TlvAttrSet_c() :
            lstAttrs_m(8)
        {}
        ~TlvAttrSet_c()
        {
            for (int nIndex=lstAttrs_m.size() - 1; nIndex >= 0; nIndex--)
            {
                TlvAttrIf_i* pItem = lstAttrs_m.takeLast();

                delete pItem;
            }
        }

        virtual bool isAtomic()             { return false; }

        ////////////////////////////////////////////////////////////////////////////////
        // sub-TLV's info to be calculated by TlvEncoder to avoid recursive calling
        ////////////////////////////////////////////////////////////////////////////////
        virtual int getValueLen()           { return 0; }
        virtual const char* getValue()      { return NULL; }

        virtual bool appendAttr(TlvAttrIf_i* pItem)
        {
            if (NULL == pItem)
            {
                return false;
            }

            lstAttrs_m.append(pItem);
            return true;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // get set iterator
        ////////////////////////////////////////////////////////////////////////////////
        virtual TlvAttrIterator_i* getIterator();
    protected:
    private:
        TempSimpleVector_c<TlvAttrIf_i*> lstAttrs_m;
};

class TlvAttrSetIterator_c : public TlvAttrIterator_i
{
    public:
        TlvAttrSetIterator_c(TlvAttrSet_c* pNode) :
            pRoot_m(pNode),
            lstAttrs_m(NULL),
            nCurIndex_m(0)
        {}
        virtual ~TlvAttrSetIterator_c() {}

        virtual TlvAttrIf_i* next()
        {
            if ((NULL == lstAttrs_m) || (nCurIndex_m >= lstAttrs_m->size()))
            {
                return NULL;
            }

            return lstAttrs_m->get(nCurIndex_m++);
        }

        inline void setList(TempSimpleVector_c<TlvAttrIf_i*>* lstAttrs)
        {
            lstAttrs_m = lstAttrs;
        }

    protected:
    private:
        TlvAttrSet_c* pRoot_m;

        TempSimpleVector_c<TlvAttrIf_i*> * lstAttrs_m;
        int nCurIndex_m;
};

#endif
