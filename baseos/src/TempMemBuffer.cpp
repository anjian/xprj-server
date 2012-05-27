#include <common/inc/MyStdLibrary.h>

#include <interface/baseos/MemoryIf.h>
#include <interface/baseos/TempMemBuffer.h>

TempMemBuffer_c::TempMemBuffer_c() :
    pBuf_m(NULL),
    nBufLen_m(0),
    nContentLen_m(0)
{
}

TempMemBuffer_c::~TempMemBuffer_c()
{
    if (NULL != pBuf_m)
    {
        TR_MEMORY_FREE(pBuf_m);
    }
}

bool TempMemBuffer_c::cutAsString()
{
    return append(0);
}

bool TempMemBuffer_c::append(char cChar)
{
    if (!extendBuffer(1))
    {
        clear();
        return false;
    }

    pBuf_m[nContentLen_m] = cChar;
    nContentLen_m++;

    return true;
}

bool TempMemBuffer_c::append(const char* pContent, int nLen)
{
    if ((NULL == pContent) || (0 == nLen))
    {
        return false;
    }

    if (!extendBuffer(nLen))
    {
        clear();
        return false;
    }

    xmemcpy(pBuf_m + nContentLen_m, pContent, nLen);
    nContentLen_m += nLen;

    return true;
}

bool TempMemBuffer_c::extendBuffer(int nLen)
{
    int nTargetLen = nContentLen_m + nLen;
    if (nTargetLen > nBufLen_m)
    {
        int nOrigSize = nBufLen_m;
        if (0 == nBufLen_m)
        {
            nBufLen_m = 32;
        }

        while(nBufLen_m < nTargetLen)
        {
            nBufLen_m <<= 1;
        }

        pBuf_m = (char*)TR_MEMORY_REALLOC(pBuf_m, nOrigSize, nBufLen_m);
    }

    return (NULL != pBuf_m);
}
