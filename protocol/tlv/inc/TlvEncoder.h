#ifndef __TLV_ENCODER_H_
#define __TLV_ENCODER_H_

#include <common/inc/SimpleVector.h>

#include <interface/baseos/TempMemBuffer.h>
#include <interface/protocol/tlv.h>
//#include <interface/protocol/TlvIf.h>

#include <protocol/tlv/inc/TlvGenerator.h>

class TlvAttrIf_i;
class TlvAttrIterator_i;

class TlvAttrStackItem_c : public TempObject_i
{
    public:
        TlvAttrStackItem_c() :
            pItem_m(NULL),
            pIterator_m(NULL)
        {
        }

        ~TlvAttrStackItem_c();

        inline void setItem(TlvAttrIf_i* pItem)             { pItem_m = pItem; }
        inline void setIterator(TlvAttrIterator_i* pIter)   { pIterator_m = pIter; }

        inline TlvAttrIf_i* getItem()                       { return pItem_m; }
        inline TlvAttrIterator_i* getIterator()             { return pIterator_m; }

        inline const char* getAttrValue()                   { return tbBuffer_m.getBuffer(); }
        inline int getAttrLength()                          { return tbBuffer_m.getLength(); }

        inline bool appendType(int nType)
        {
            return TlvGenerator_c::appendType(tbBuffer_m, (TlvAttrType_e)nType);
        }

        inline bool appendLength(int nLength)
        {
            return TlvGenerator_c::appendLength(tbBuffer_m, nLength);
        }

        inline bool append(const char* pBuf, int nLen)
        {
            return TlvGenerator_c::appendValue(tbBuffer_m, pBuf, nLen);
        }

        bool appendAttr(TlvAttrIf_i* pItem);

    private:
        TlvAttrIf_i* pItem_m;
        TlvAttrIterator_i* pIterator_m;

        TempMemBuffer_c tbBuffer_m;
};

#if 0
class TlvEncoder_c : public TempObject_i
{
    public:
        TlvEncoder_c() :
            lstAttrSetStack_m(8)
        {}
        virtual ~TlvEncoder_c();

        bool generate(TempSimpleVector_c<TlvAttrIf_i*>* ppAttr, TempMemBuffer_c& tbOutput);
        bool generate(TlvAttrIf_i* pAttr, TempMemBuffer_c& tbOutput);

    protected:
    private:
        TempSimpleVector_c<TlvAttrStackItem_c*> lstAttrSetStack_m;
};
#endif

#endif
