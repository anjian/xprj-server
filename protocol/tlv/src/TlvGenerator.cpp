
#include <protocol/tlv/inc/TlvGenerator.h>

bool TlvGenerator_c::appendType(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType)
{
    return appendShort(tbBuffer, eType);
}

bool TlvGenerator_c::appendLength(TempMemBuffer_c& tbBuffer, int nLength)
{
    return appendShort(tbBuffer, nLength);
}

bool TlvGenerator_c::appendValue(TempMemBuffer_c& tbBuffer, const char* pValue, int nLen)
{
    return appendBuf(tbBuffer, pValue, nLen);
}


bool TlvGenerator_c::append(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType, int nLength, const char* pValue)
{
    appendShort(tbBuffer, eType);
    appendShort(tbBuffer, nLength);
    return appendBuf(tbBuffer, pValue, nLength);
}

bool TlvGenerator_c::append(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType, int nValue)
{
    appendShort(tbBuffer, eType);
    appendShort(tbBuffer, TLV_LEN_TYPE_INT32);
    appendInt32(tbBuffer, nValue);

    return true;
}

bool TlvGenerator_c::append(TempMemBuffer_c& tbBuffer, TlvAttrType_e eType, long long nValue)
{
    appendShort(tbBuffer, eType);
    appendShort(tbBuffer, TLV_LEN_TYPE_INT64);
    appendInt64(tbBuffer, nValue);

    return true;
}

bool TlvGenerator_c::appendByte(TempMemBuffer_c& tbBuffer, int nValue)
{
    tbBuffer.append((char)(nValue & 0x0FF));

    return true;
}

bool TlvGenerator_c::appendShort(TempMemBuffer_c& tbBuffer, int nValue)
{
    tbBuffer.append((char)(nValue & 0x0FF));
    tbBuffer.append((char)((nValue >> 8) & 0x0FF));

    return true;
}

bool TlvGenerator_c::appendInt32(TempMemBuffer_c& tbBuffer, int nValue)
{
    tbBuffer.append((char)(nValue & 0x0FF));
    tbBuffer.append((char)((nValue >> 8) & 0x0FF));
    tbBuffer.append((char)((nValue >> 16) & 0x0FF));
    tbBuffer.append((char)((nValue >> 24) & 0x0FF));

    return true;
}

bool TlvGenerator_c::appendInt64(TempMemBuffer_c& tbBuffer, long long nValue)
{
    tbBuffer.append((char)(nValue & 0x0FF));
    tbBuffer.append((char)((nValue >> 8) & 0x0FF));
    tbBuffer.append((char)((nValue >> 16) & 0x0FF));
    tbBuffer.append((char)((nValue >> 24) & 0x0FF));
    tbBuffer.append((char)((nValue >> 32) & 0x0FF));
    tbBuffer.append((char)((nValue >> 40) & 0x0FF));
    tbBuffer.append((char)((nValue >> 48) & 0x0FF));
    tbBuffer.append((char)((nValue >> 56) & 0x0FF));

    return true;
}

bool TlvGenerator_c::appendBuf(TempMemBuffer_c& tbBuffer, const char* pBuf, int nLen)
{
    return tbBuffer.append(pBuf, nLen);
}
