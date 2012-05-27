#ifndef __TLV_IF_H_
#define __TLV_IF_H_

#include <common/inc/SimpleVector.h>

#include <interface/baseos/TempObject.h>
#include <interface/baseos/TempMemBuffer.h>

#include <interface/protocol/tlv.h>

typedef void (*TlvAttributeHandlerCB)(void *handler, int nType, int nLen, const void* pData);

class TlvAttrIterator_i;
class TlvAttrIf_i;

TlvAttrIf_i* createTlvAttribute(int nType);

class TlvAttrIf_i : public TempObject_i
{
    public:
        TlvAttrIf_i() :
            nType_m(0)            // TODO: to replaced by enum
        {
        }
        virtual ~TlvAttrIf_i()      {}

        inline void setType(int nType)  { nType_m = nType; }
        inline int getType()            { return nType_m; }

        ////////////////////////////////////////////////////////////////////////////////
        // check if attribute is atom, and has no sub-attribute
        ////////////////////////////////////////////////////////////////////////////////
        virtual bool isAtomic()         { return true; }

        inline void cutAsString()       { tbAttr_m.cutAsString(); }

        virtual const char* getValue()  { return tbAttr_m.getBuffer(); }
        virtual int getValueLen()       { return tbAttr_m.getLength(); }

        virtual bool appendAttr(TlvAttrIf_i* /*pItem*/)
        {
            return false;
        }

        virtual TlvAttrIterator_i* getIterator()    { return NULL; }

        ////////////////////////////////////////////////////////////////////////////////
        // TODO:
        //
        // the performance is not good since the content has to be copied too many times,
        // to be refined
        ////////////////////////////////////////////////////////////////////////////////
        bool setValue_byte(char cValue);
        bool setValue_short(int nValue);
        bool setValue_int32(int nValue);
        bool setValue_int64(long long lValue);
        bool setValue_buffer(const char* sVal, int nLen);

        int getByte();
        int getShort();
        int getInt32();
        long long getInt64();
        const char* getString();

    protected:
        int getInt_N(const char* pBuf, int nBytes);

    private:
        int nType_m;

        // only keep serialized attribute
        TempMemBuffer_c tbAttr_m;
};

class TlvAttrIterator_i : public TempObject_i
{
    public:
        virtual ~TlvAttrIterator_i() {}

        virtual TlvAttrIf_i* next() = 0;

    protected:
    private:
};

enum TlvParserState_e
{
    TLV_PARSE_DONE,
    TLV_PARSE_CONTINUE,
    TLV_PARSE_FAIL,
};

class TlvAttrStackItem_c;
class TlvEncoder_c
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

class TlvParserStackItem_c;
class TlvParser_c
{
    public:
        TlvParser_c(SimpleVector<TlvAttrIf_i*>* pAttrsOutput) :
            lstAttrSetStack_m(8),
            lstAttrs_m(pAttrsOutput)
        {
        }

        ~TlvParser_c();

        bool init();

        static bool isMessageCompleted(const char* pBuf, int nLen);

        TlvParserState_e parse(const char* pBuf, int nLen);

    protected:
        static int checkLength(const char * pBuf);
        static int getShort(const char* pBuf);
        static int getInt32(const char* pBuf);
        static int getInt64(const char* pBuf);
        static int getInt_N(const char* pBuf, int nBytes);
        //int getString(const char* pBuf, int nLength);

    private:
        TempMemBuffer_c tbReceivedBuf_m;

        TempSimpleVector_c<TlvParserStackItem_c*> lstAttrSetStack_m;

        // output, set externally
        SimpleVector<TlvAttrIf_i*>* lstAttrs_m;
};



#endif
