#ifndef __TLV_GENERATOR_H_
#define __TLV_GENERATOR_H_

#include <interface/baseos/TempMemBuffer.h>
#include <interface/protocol/tlv.h>

class TlvGenerator_c
{
    public:
        TlvGenerator_c()        {}
        ~TlvGenerator_c()       {}

#if 0
        inline char* getBuffer()        { return tbBuffer_m.getBuffer(); }
        inline int getLength()          { return tbBuffer_m.getLength(); }
#endif

        static bool appendType(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType);
        static bool appendLength(TempMemBuffer_c& tbBuffer, int nLength);
        static bool appendValue(TempMemBuffer_c& tbBuffer, const char* pValue, int nLen);

        static bool append(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType, int nLength, const char* pValue);
        static bool append(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType, int nValue);
        static bool append(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType, long long nValue);

        static bool appendByte(TempMemBuffer_c& tbBuffer, int nValue);
        static bool appendShort(TempMemBuffer_c& tbBuffer, int nValue);
        static bool appendInt32(TempMemBuffer_c& tbBuffer, int nValue);
        static bool appendInt64(TempMemBuffer_c& tbBuffer, long long nValue);
        static bool appendBuf(TempMemBuffer_c& tbBuffer, const char* pBuf, int nLen);

    protected:
    private:
        //TempMemBuffer_c tbBuffer_m;
};

#endif
