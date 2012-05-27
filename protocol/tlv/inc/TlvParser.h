#ifndef __TLV_PARSER_H_
#define __TLV_PARSER_H_

#include <common/inc/SimpleVector.h>

#include <interface/baseos/TempMemBuffer.h>

#include <interface/protocol/tlv.h>

#if 0
class TlvAttribute_c : public TempObject_i
{
    public:
        TlvAttribute_c() :
            eType_m(MSG_TYPE_UNKNOWN),
            nLength_m(0)
        {
        }
        virtual ~TlvAttribute_c();

        inline int getType()      { return eType_m; }
        inline void setType(int nType)
        {
            eType_m = ((nType <= MSG_TYPE_START) || (nType >= MSG_TYPE_END)) ? MSG_TYPE_UNKNOWN : nType;
        }

        inline int getLength()              { return nLength_m; }
        inline void setLength(int nLen)     { nLength_m = nLen; }

        inline const char* getData()        { return mbValue_m.getBuffer(); }
        inline void appendData(const char* pBuf, int nLen)
        {
            mbValue_m.append(pBuf, nLen);
        }

    protected:
    private:
        int eType_m;
        int nLength_m;
        TempMemBuffer_c mbValue_m;
};
#endif

class TlvParserStackItem_c : public TempObject_i
{
    public:
        TlvParserStackItem_c() :
            pAttr_m(NULL),
            pNewHeader_m(NULL),
            nLeftLen_m(0)
        {
        }
        virtual ~TlvParserStackItem_c() {}

        inline void setAttr(TlvAttrIf_i* pAttr)     { pAttr_m = pAttr; }
        inline TlvAttrIf_i* getAttr()               { return pAttr_m; }

        inline bool appendAttr(TlvAttrIf_i* pAttr)  { return pAttr_m->appendAttr(pAttr); }

        inline void setNextHeader(const char* pHeader)  { pNewHeader_m = pHeader; }
        inline void setLeftLen(int nLen)            { nLeftLen_m = nLen; }

        inline const char* getNextHeader()          { return pNewHeader_m; }
        inline int getLeftLen()                     { return nLeftLen_m; } 

    protected:

    private:
        TlvAttrIf_i* pAttr_m;

        // To store the scene after this unit is completed
        const char* pNewHeader_m;
        int nLeftLen_m;
};

#if 0
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

        TlvParserState_e parse(const char* pBuf, int nLen);

    protected:
        int checkLength(const char * pBuf);
        int getShort(const char* pBuf);
        int getInt32(const char* pBuf);
        int getInt64(const char* pBuf);
        int getInt_N(const char* pBuf, int nBytes);
        //int getString(const char* pBuf, int nLength);

    private:
        TempMemBuffer_c tbReceivedBuf_m;

        TempSimpleVector_c<TlvParserStackItem_c*> lstAttrSetStack_m;

        // output, set externally
        SimpleVector<TlvAttrIf_i*>* lstAttrs_m;
};
#endif

#endif
