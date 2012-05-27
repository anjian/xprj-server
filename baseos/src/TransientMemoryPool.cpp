
#include <common/inc/os_type.h>

#include <common/inc/MyDebug.h>
#include <common/inc/MySystem.h>
#include <common/inc/MyStdLibrary.h>

#include <interface/system/GlobalInstanceMgr.h>
#include <interface/baseos/MemoryIf.h>

#include <baseos/inc/TransientMemoryPool.h>

////////////////////////////////////////////////////////////////////////////////
// external interface: interface/baseos/TransientMemory.h
////////////////////////////////////////////////////////////////////////////////
//
#include <interface/baseos/TransientMemory.h>

void* TR_MEMORY_ALLOC(size_t size)
{
    return TransientMemoryPool_c::getInstance()->trAlloc(size);
}

void* TR_MEMORY_REALLOC(void* ptr, size_t nOrigSize, size_t nNewSize)
{
    return TransientMemoryPool_c::getInstance()->trResize(ptr, nOrigSize, nNewSize);
}

void TR_MEMORY_FREE(void* ptr)
{
    TransientMemoryPool_c::getInstance()->trFree(ptr);
}
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// memory pool info implementation
////////////////////////////////////////////////////////////////////////////////
//
int TransientMemoryPool_c::nOccupiedBlockCount_m = 0;;

TransientMemoryPoolInfo_c::TransientMemoryPoolInfo_c() 
{
    pTrPoolPtr_m        = NULL;
    nStartSegment_m     = -1;
    nSegmentNumber_m    = 0;
    nBlockNumber_m      = -1;
    pBlockUsageFlag_m   = NULL;
    ePoolType_m         = E_TRMEM_POOL_UNKNOWN;
}

TransientMemoryPoolInfo_c::~TransientMemoryPoolInfo_c() 
{
    if (NULL != pBlockUsageFlag_m)
    {
        delete pBlockUsageFlag_m;
        pBlockUsageFlag_m    = NULL;
    }
}

void TransientMemoryPoolInfo_c::setMemoryPoolInfo(char *pPool, int nStartSeg, int nSegNum, TransientMemoryPoolType_e ePoolType)
{
    pTrPoolPtr_m        = pPool;
    nStartSegment_m     = nStartSeg;
    nSegmentNumber_m    = nSegNum;
    ePoolType_m         = ePoolType;

    // get the number of character needed to keep usage flags for each block
    //
    nBlockNumber_m      = ((((0x01 << TR_MEMORY_SEGMENT_SHIFT) - 1) >> ePoolType_m) + 1) * nSegNum;

    MSG_TRMEM("Pool init: type [%d] block [%d]\n", (0x01 << ePoolType_m), nBlockNumber_m);

    // bytes needed for keeping usage flag
    nUsageFlagLen_m     = nBlockNumber_m >> 3;
    if (nBlockNumber_m & 0x07)
    {
        nUsageFlagLen_m ++;
    }

    pBlockUsageFlag_m   = new char [nUsageFlagLen_m];
    if (NULL == pBlockUsageFlag_m)
    {
        return;
    }

    xmemset(pBlockUsageFlag_m, 0, nUsageFlagLen_m);

    // set the last un-used flag to 1 (occupied)
    if (nBlockNumber_m & 0x07)
    {
        for (int nInd=0; nInd<(0x08 - (nBlockNumber_m & 0x07)); nInd++)
        {
            pBlockUsageFlag_m[nUsageFlagLen_m-1] |= (0x80 >> nInd);
        }
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
//
//         Bit:      7   6   5   4   3   2   1   0
//
// Block Index:   7 + n*8                     0 + n*8
//                 ---------------------------------
//  *Flags: n=0    |   |   |   |   |   |   |   |   |
//                 ---------------------------------
//          n=1    |   |   |   |   |   |   |   |   |
//                 ---------------------------------
//                         ...
//                 ---------------------------------
//          n=N    |   |   |   |   |   |   |   |   |
//                 ---------------------------------
//
////////////////////////////////////////////////////////////////////////////////
void* TransientMemoryPoolInfo_c::getTrMemBlock(int& nIndex)
{
    // To support both 32 and 64 bits
    static long LONG_MASK       = -1L;            // all bits are turned on
    static int BYTES_OF_LONG    = sizeof(long);

    int nFlagCharOffset  = 0;
    bool bFoundAvailable = false;

    long* pFlagByInt = (long*)pBlockUsageFlag_m;
    char* pFlagByChar = pBlockUsageFlag_m;

    //int nBitsOfLong     = (BYTES_OF_LONG << 3);

    // 1. search free block by integer first
    //
    // TODO: there is a division here, should be a good to remove it rather than depending
    //      on compiler optimization:
    //           32B => N << 5
    //           64B => N << 6
    for (int nLongIndex=0;
            nLongIndex<(nBlockNumber_m / (BYTES_OF_LONG << 3));
            nLongIndex++)
    {
        if (LONG_MASK != (*pFlagByInt & LONG_MASK))
        {
            // some blocks are available...
            bFoundAvailable = true;
            pFlagByChar     = (char*) pFlagByChar;

            break;
        }

        pFlagByInt ++;
        nFlagCharOffset += BYTES_OF_LONG;
    }

    int nByteCheckNum   = BYTES_OF_LONG;
    if (false == bFoundAvailable) 
    {
        nByteCheckNum = (nUsageFlagLen_m & (BYTES_OF_LONG - 1));

        if (0 >= nByteCheckNum)
        {
            return NULL;
        }
    }

    // 2. Get char index
    int nFlagCharCheckEnd   = nFlagCharOffset + nByteCheckNum;
    for (; nFlagCharOffset < nFlagCharCheckEnd; nFlagCharOffset++)
    {
        if (0x0FF != (pBlockUsageFlag_m[nFlagCharOffset] & 0x0FF))
        {
            bFoundAvailable = true;
            break;
        }
    }

    // 3. get bit index
    if (bFoundAvailable) 
    {
        char cBitMask = 0x01;
        for (int nBitInd=0; nBitInd<8; nBitInd++)
        {
            if (0 == (pBlockUsageFlag_m[nFlagCharOffset] & cBitMask))
            {
                // Set the flag as occupied
                pBlockUsageFlag_m[nFlagCharOffset] |= cBitMask;

                // Available block is found
                nIndex = (nFlagCharOffset << 3) + nBitInd;

                return (pTrPoolPtr_m + (nIndex << ePoolType_m));
            }

            // shift bit mask
            cBitMask <<= 1;
        }
    }

    return NULL;
}

void TransientMemoryPoolInfo_c::freeTrMemBlock(void* pPtr)
{
    int pPtrOffset      = (char*)pPtr - pTrPoolPtr_m;

    // From performance perspective, this check may not need... 
    if ((pPtrOffset < 0) || (pPtrOffset >= (nSegmentNumber_m << TR_MEMORY_SEGMENT_SHIFT)))
    {
        MSG_TRMEM("Pointer is out of scope\n");
        return;
    }

    int nBlockIndex = (pPtrOffset >> ePoolType_m);

    // Reset corresponding bit to FREE (0)
    unsigned char cBitMask = ~(0x01 << (nBlockIndex & 0x07));
    pBlockUsageFlag_m[(nBlockIndex >> 3)] &= cBitMask;

    return;
}

////////////////////////////////////////////////////////////////////////////////
// Transient memory pool implementation
////////////////////////////////////////////////////////////////////////////////
//
TransientMemoryPool_c::TransientMemoryPool_c()
{
    // Init the segment number of each pool
    initSegmentDefaultAssignment();

    // Get the sum of segments
    nMaxSegmentNumber_m = 0;
    for (int ePoolInd=E_TRMEM_POOL_START; ePoolInd<E_TRMEM_POOL_END; ePoolInd++)
    {
        nMaxSegmentNumber_m += nDefaultSegNum_m[ePoolInd];
    }

    // allocate whole memory for pools
    pTrPool_m = new char[nMaxSegmentNumber_m << TR_MEMORY_SEGMENT_SHIFT];

    // assign the header for pools
    char* pNextHeader   = pTrPool_m;
    int nSegStart       = 0;
    for (int ePoolInd=E_TRMEM_POOL_START; ePoolInd<E_TRMEM_POOL_END; ePoolInd++)
    {
        if (nDefaultSegNum_m[ePoolInd] > 0)
        {
            trmSegPool_m[ePoolInd].setMemoryPoolInfo(pNextHeader, 
                    nSegStart, 
                    nDefaultSegNum_m[ePoolInd], 
                    (TransientMemoryPoolType_e)ePoolInd);

            pNextHeader += (trmSegPool_m[ePoolInd].nSegmentNumber_m << TR_MEMORY_SEGMENT_SHIFT);
            nSegStart += nDefaultSegNum_m[ePoolInd];
        }
    }
}

TransientMemoryPool_c::~TransientMemoryPool_c()
{
    if (NULL != pTrPool_m)
    {
        delete[] (char*)pTrPool_m;
        pTrPool_m = NULL;
    }

}

IMPLE_OBJECT_GET_INSTANCE(TransientMemoryPool_c)

void TransientMemoryPool_c::initSegmentDefaultAssignment()
{
    // Adjust nShiftWidth to change pool segment allocation
    int nShiftWidth = 1;

    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_4]     =  0;//2 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_8]     =  0;//2 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_16]    =  0;//2 << nShiftWidth;

    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_32]    =  1 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_64]    =  1 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_128]   =  1 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_256]   =  1 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_512]   =  1 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_1024]  =  1 << nShiftWidth;
    nDefaultSegNum_m[E_TRMEM_POOL_BYTE_2048]  =  1 << nShiftWidth;
}

int TransientMemoryPool_c::getPtrSize(void* ptr)
{
    TransientMemoryPoolType_e ePoolType = E_TRMEM_POOL_UNKNOWN;
    getBlockIndex(ptr, ePoolType);

    return (E_TRMEM_POOL_UNKNOWN != ePoolType) ?  (0x01 << ePoolType) : 0;
}

int TransientMemoryPool_c::getBlockIndex(void * pPtr, TransientMemoryPoolType_e& ePoolType)
{
    ePoolType           = E_TRMEM_POOL_UNKNOWN;
    int pPtrOffset      = (char*)pPtr - pTrPool_m;

    if ((pPtrOffset < 0) || (pPtrOffset >= (nMaxSegmentNumber_m << TR_MEMORY_SEGMENT_SHIFT)))
    {
        MSG_TRMEM("Pointer is out of scope\n");
        return -1;
    }

    int nSegmentIndex   = pPtrOffset >> TR_MEMORY_SEGMENT_SHIFT;
    int nBlockIndex     = -1;

    int ePoolInd;
    for (ePoolInd=E_TRMEM_POOL_START; ePoolInd<E_TRMEM_POOL_END; ePoolInd++)
    {
        if (trmSegPool_m[ePoolInd].includeSegment(nSegmentIndex))
        {
            nBlockIndex = (pPtrOffset >> ePoolInd);
            break;
        }
    }

    if (-1 == nBlockIndex)
    {
        MSG_TRMEM_WARN("segment index is out of range\n");
    }
    else
    {
        ePoolType = (TransientMemoryPoolType_e)ePoolInd;
    }

    return nBlockIndex;
}

void* TransientMemoryPool_c::trAlloc(size_t nSize)
{
#ifndef NDEBUG
    return xmalloc(nSize);
#else

    if (nSize > (0x01 << (E_TRMEM_POOL_END - 1)))
    {
        nOccupiedBlockCount_m++;
        MSG_TRMEM_WARN("[%d] Too big, allocate by system directly, Total Occupied [%d]\n", nSize, nOccupiedBlockCount_m);

        return ((void*)new char[nSize]);
    }

    // lock the pool
    //mutex_m->lock();

    int nBlockIndex     = 0;
    void* pTrMemBlock   = NULL;

    for (int ePoolType=E_TRMEM_POOL_START; ePoolType<E_TRMEM_POOL_END; ePoolType++)
    {
        if ((nSize <= (unsigned int)(0x01 << ePoolType)) && ( trmSegPool_m[ePoolType].isAvaiable()))
        {
            pTrMemBlock = trmSegPool_m[ePoolType].getTrMemBlock(nBlockIndex);
            if (NULL != pTrMemBlock)
            {
                nOccupiedBlockCount_m++;

                MSG_TRMEM("Allocate [%d bytes] [%08X] block for request [%d]. Total Occupied [%d]\n", (0x01<<ePoolType), pTrMemBlock, nSize, nOccupiedBlockCount_m);
                //return pTrMemBlock;
                break;
            }
            else
            {
                MSG_TRMEM_WARN("[%d bytes] pool exhausted, try next one...\n", (0x01<<ePoolType));
            }
        }
    }

    if (NULL == pTrMemBlock)
    {
        nOccupiedBlockCount_m++;
        //MSG_TRMEM_ERR("Could not allocate from pool, try system memory for size[%d]. Total occupied [%d]\n",  nSize, nOccupiedBlockCount_m);

        pTrMemBlock =  (void*)new char[nSize];
    }

    // unlock the pool
    //mutex_m->unlock();

    return pTrMemBlock;
#endif
}

#if 0
////////////////////////////////////////////////////////////////////////////////
// WARNNING WARNNING WARNNING
//
//  Be careful to use this trResize, it can ONLY work properly when pool is big
//  enough. If system can not find original one from pool, it has no way to get
//  the memory block size, that means it will not be able to copy original content
//  to new buffer
////////////////////////////////////////////////////////////////////////////////
void* TransientMemoryPool_c::trResize(void* pPtr, int nNewSize)
{
    int nOrigSize = getPtrSize(pPtr);
    if ((0 >= nNewSize) || ((0 < nNewSize) && (nOrigSize >= nNewSize)))
    {
        // if new request size is smaller than original one, return original one directly
        return pPtr;
    }

    // allocate new memory block
    void* pNew = trAlloc(nNewSize);
    if ((NULL != pNew) && (nOrigSize > 0))
    {
        memcpy(pNew, pPtr, nOrigSize);
    }
    else
    {
        MSG_TRMEM_ERR("Copy orignal content error length=%d\n", nOrigSize);
    }

    // release old memory
    trFree(pPtr);

    return pNew;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// foolish resize
////////////////////////////////////////////////////////////////////////////////
void* TransientMemoryPool_c::trResize(void* pPtr, size_t nOrigSize, size_t nNewSize)
{
#ifndef NDEBUG
    return xrealloc(pPtr, nNewSize, nOrigSize);
#else
    if ((0 >= nNewSize) || ((0 < nNewSize) && (nOrigSize >= nNewSize)))
    {
        // if new request size is smaller than original one, return original one directly
        return pPtr;
    }

    // allocate new memory block
    void* pNew = trAlloc(nNewSize);
    if ((NULL != pNew) && (nOrigSize > 0))
    {
        xmemcpy(pNew, pPtr, nOrigSize);
    }
    else
    {
        MSG_TRMEM_WARN("Copy orignal content error length=%d\n", nOrigSize);
        //MSG_TRMEM_ERR("Copy orignal content error length=%d\n", nOrigSize);
    }

    // release old memory
    trFree(pPtr);

    return pNew;
#endif
}

void TransientMemoryPool_c::trFree(void* pPtr)
{
    if (NULL == pPtr)
    {
        return;
    }

#ifndef NDEBUG
    xfree(pPtr);
#else

    int pPtrOffset = (char*)pPtr - pTrPool_m;
    if ((pPtrOffset < 0) || (pPtrOffset >= (nMaxSegmentNumber_m << TR_MEMORY_SEGMENT_SHIFT)))
    {
        nOccupiedBlockCount_m--;

        MSG_TRMEM_WARN("delete system allocated memory. Total occupied [%d]\n", nOccupiedBlockCount_m);
        delete[] (char *)pPtr;
    }
    else
    {
        int nSegmentIndex   = pPtrOffset >> TR_MEMORY_SEGMENT_SHIFT;

        //
        for (int ePoolType=E_TRMEM_POOL_START; ePoolType<E_TRMEM_POOL_END; ePoolType++)
        {
            if (trmSegPool_m[ePoolType].includeSegment(nSegmentIndex))
            {
                nOccupiedBlockCount_m--;
                MSG_TRMEM("release memory block [%d bytes] [%08X]. Total occupied [%d]\n", (0x01<<ePoolType), pPtr, nOccupiedBlockCount_m);

                trmSegPool_m[ePoolType].freeTrMemBlock(pPtr);
                break;
            }
        }
    }
#endif

    return;
}

#if 0
// Test code
int main (int argc, char* argv[])
{
    TransientMemoryPool_c* pTrPool = TransientMemoryPool_c::getInstance();

    void * pTrm[65];

    for (int i=0; i<3; i++)
    {
        MSG_TRMEM("Round %d\n", i);

        for (int j=0; j<10; j++)
        {
            pTrm[j]     = pTrPool->trAlloc(6);

            sprintf ((char*)pTrm[j],
                    "%X i=%02d, j=%02d,", //Give a length is 64 string abcdefa add",
                    (unsigned int)(pTrm[j]), i, j);
        }

        pTrm[3] = pTrPool->trResize(pTrm[3], 32, 65);
        pTrm[8] = pTrPool->trResize(pTrm[8], 32, 150);

        for (int j=10; j<13; j++)
        {
            pTrm[j]     = pTrPool->trAlloc(6);

            sprintf ((char*)pTrm[j],
                    "%X i=%02d, j=%02d,", //Give a length is 64 string abcdefa add",
                    (unsigned int)(pTrm[j]), i, j);
        }

        for (int j=0; j<13; j++)
        {
            MSG_TRMEM("%X: %s\n", (unsigned int)(pTrm[j]), (char*)pTrm[j]);
        }

        for (int j=12; j>=0; j--)
        {
            pTrPool->trFree(pTrm[j]);
        }
    }

    return 0;
}
#endif
