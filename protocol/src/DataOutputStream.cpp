#include <common/inc/MyStdLibrary.h>

#include <interface/protocol/DataOutputStream.h>

bool UcWebDataOutputStream_c::appendByte(char* sBuf, const int nBufLen, int& nContentLen, char cContent)
{
    if ((NULL == sBuf) || (nContentLen >= nBufLen))
    {
        return false;
    }

    sBuf[nContentLen++] = cContent;
    return true;
}

bool UcWebDataOutputStream_c::appendInt16(char* sBuf, const int nBufLen, int& nContentLen, int nNum)
{
    if ((NULL == sBuf) || (nContentLen + 1 >= nBufLen))
    {
        return false;
    }

    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 8) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)(nNum & 0xFF);

    return true;
}

bool UcWebDataOutputStream_c::appendInt32(char* sBuf, const int nBufLen, int& nContentLen, int nNum)
{
    if ((NULL == sBuf) || (nContentLen + 3 >= nBufLen))
    {
        return false;
    }

    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 24) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 16) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 8) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)(nNum & 0xFF);

    return true;
}

bool UcWebDataOutputStream_c::appendInt64(char* sBuf, const int nBufLen, int& nContentLen, long long nNum)
{
    if ((NULL == sBuf) || (nContentLen + 7 >= nBufLen))
    {
        return false;
    }

    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 56) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 48) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 40) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 32) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 24) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 16) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)((nNum >> 8) & 0xFF);
    sBuf[nContentLen++] = (UC_CHAR)(nNum & 0xFF);

    return true;
}

bool UcWebDataOutputStream_c::appendString(char* sBuf, const int nBufLen, int& nContentLen, char* sStr, int nLen)
{
    if ((NULL == sBuf) || (NULL == sStr) || (nContentLen + nLen - 1 >= nBufLen))
    {
        return false;
    }

    if (nLen <= 0)
    {
        return true;
    }

    xmemcpy(sBuf + nContentLen, sStr, nLen);
    nContentLen += nLen;

    return true;
}
