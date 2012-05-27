
#include <common/inc/MyStdLibrary.h>

#include <interface/protocol/TlvIf.h>

#include <protocol/tlv/inc/TlvParser.h>
#include <protocol/tlv/inc/TlvAttrSet.h>

const int TLV_TYPE_BYTES    = 2;
const int TLV_LENGTH_BYTES  = 2;

TlvParser_c::~TlvParser_c()
{
    for (int nIndex=lstAttrSetStack_m.size()-1; nIndex>=0; nIndex--)
    {
        delete lstAttrSetStack_m.takeLast();
    }
}

bool TlvParser_c::init()
{
    return true;
}

bool TlvParser_c::isMessageCompleted(const char* pBuf, int nLen)
{
    const char* pPtr    = pBuf;
    int nLeftLen        = nLen;

    // try to find TAIL attribute
    for (;;)
    {
        // check if attribute header (type + length) is received
        if (nLeftLen <= TLV_TYPE_BYTES + TLV_LENGTH_BYTES)
        {
            break;
        }

        int nAttrLen = checkLength(pPtr);
        int nAttrTotalLen = TLV_TYPE_BYTES + TLV_LENGTH_BYTES + nAttrLen;

        int nType = getShort(pPtr);
        if (TLV_ATTR_TAIL_FLAG == nType)
        {
            return true;
        }

        // check if whole attribute is received
        if (nLeftLen < nAttrTotalLen)
        {
            break;
        }

        pPtr += nAttrTotalLen;
        nLeftLen -= nAttrTotalLen;
    }

    return false;
}

TlvParserState_e TlvParser_c::parse(const char* pBuf, int nLen)
{
    TlvParserState_e eRet   = TLV_PARSE_CONTINUE;

    const char* pPtr    = pBuf;
    int nLeftLen        = nLen;

    // indicate if pPtr is from received buffer
    bool bNeedCache     = false;
    // combine previous left content if there is
    if (0 < tbReceivedBuf_m.getLength())
    {
        tbReceivedBuf_m.append(pBuf, nLen);

        pPtr        = tbReceivedBuf_m.getBuffer();
        nLeftLen    = tbReceivedBuf_m.getLength();

        bNeedCache  = true;
    }

    for (;;)
    {
        if ((0 >= nLeftLen) && (0 < lstAttrSetStack_m.size()))
        {
            TlvParserStackItem_c* pStackItem = lstAttrSetStack_m.takeLast();

            pPtr        = pStackItem->getNextHeader();
            nLeftLen    = pStackItem->getLeftLen();

            delete pStackItem;
        }

        // check if attribute header (type + length) is received
        if (nLeftLen <= TLV_TYPE_BYTES + TLV_LENGTH_BYTES)
        {
            eRet = TLV_PARSE_CONTINUE;
            break;
        }

        int nAttrLen = checkLength(pPtr);
        int nAttrTotalLen = TLV_TYPE_BYTES + TLV_LENGTH_BYTES + nAttrLen;

        int nType = getShort(pPtr);

        // check if tail attribute is received
        if (TLV_ATTR_TAIL_FLAG == nType)
        {
            // TODO: message consistency check, to add some checking mechnism (i.e. CRC) to ensure 
            //       external message
            eRet = TLV_PARSE_DONE;
            break;
        }

        // check if whole attribute is received
        if (nLeftLen < nAttrTotalLen)
        {
            eRet = TLV_PARSE_CONTINUE;
            break;
        }

        // notify owner one attribute is gotten 
        //if ((NULL != pHandler_m) && (NULL != funcTlvAttrHandlerCb))
        {
            //funcTlvAttrHandlerCb(pHandler_m, nType, nAttrLen, pPtr + TLV_TYPE_BYTES + TLV_LENGTH_BYTES);
            TlvAttrIf_i* pAttr = NULL;

            // create attribute
            if (isAtomicTlvAttribute(nType))
            {
                pAttr = new TlvAttrIf_i();
                if (NULL == pAttr)
                {
                    return TLV_PARSE_FAIL;
                }
                pAttr->setType(nType);
                pAttr->setValue_buffer(pPtr + TLV_TYPE_BYTES + TLV_LENGTH_BYTES, nAttrLen);

            }
            else
            {
                pAttr = new TlvAttrSet_c();
                if (NULL == pAttr)
                {
                    return TLV_PARSE_FAIL;
                }

                pAttr->setType(nType);
            }

            // append new attribute
            if (lstAttrSetStack_m.size() > 0)
            {
                lstAttrSetStack_m.getLast()->appendAttr(pAttr);
            }
            else
            {
                // at root level, append into output list
                lstAttrs_m->append(pAttr);
            }

            // create stack item for attribute set
            if (!isAtomicTlvAttribute(nType))
            {
                TlvParserStackItem_c* pStackItem = new TlvParserStackItem_c();
                if (NULL == pStackItem)
                {
                    return TLV_PARSE_FAIL;
                }

                pStackItem->setAttr(pAttr);
                pStackItem->setNextHeader(pPtr + nAttrTotalLen);
                pStackItem->setLeftLen(nLeftLen - nAttrTotalLen);

                lstAttrSetStack_m.append(pStackItem);

                pPtr += (TLV_TYPE_BYTES + TLV_LENGTH_BYTES);
                nLeftLen = nAttrLen;
            }
            else
            {
                pPtr += nAttrTotalLen;
                nLeftLen -= nAttrTotalLen;
            }
        }
    }

    if ((TLV_PARSE_CONTINUE == eRet) && (nLeftLen > 0))
    {
        if (bNeedCache)
        {
            char * pTempBuf = (char*)TR_MEMORY_ALLOC(nLeftLen);
            if (NULL == pTempBuf)
            {
                return TLV_PARSE_FAIL;
            }

            xmemcpy(pTempBuf, pPtr, nLeftLen);

            tbReceivedBuf_m.clear();
            tbReceivedBuf_m.append(pTempBuf, nLeftLen);

            TR_MEMORY_FREE(pTempBuf);
        }
        else
        {
            // buffer is empty, append left content directly
            tbReceivedBuf_m.append(pPtr, nLeftLen);
        }
    }
    else
    {
        tbReceivedBuf_m.clear();
    }

    return eRet;
}

int TlvParser_c::checkLength(const char * pBuf)
{
    // shift type bytes
    return getShort(pBuf + TLV_LEN_TYPE_SHORT);
}

int TlvParser_c::getShort(const char* pBuf)
{
    return getInt_N(pBuf, TLV_LEN_TYPE_SHORT);
}

int TlvParser_c::getInt32(const char* pBuf)
{
    return getInt_N(pBuf, TLV_LEN_TYPE_INT32);
}

int TlvParser_c::getInt64(const char* pBuf)
{
    return getInt_N(pBuf, TLV_LEN_TYPE_INT64);
}

int TlvParser_c::getInt_N(const char* pBuf, int nBytes)
{
    int nRet = 0;

    for (int nIndex=nBytes - 1; nIndex>=0; nIndex--)
    {
        nRet <<= 8;
        nRet += pBuf[nIndex] & 0xFF;
    }

    return nRet;
}

#if 0
int TlvParser_c::getString(const char* pBuf, int nLength)
{
}
#endif
