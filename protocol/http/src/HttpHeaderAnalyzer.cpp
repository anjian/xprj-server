
#include <common/inc/MyStdLibrary.h>
#include <common/inc/MyCType.h>
//#include <com/inc/UcLib.h>
#include <common/inc/MyDebug.h>
//#include <com/inc/UcSystem.h>

#include <interface/baseos/TransientMemory.h>
//#include <interface/protocol/UcGZipDecoder.h>
//#include <interface/protocol/UcCharsetDecoder.h>

#include "protocol/http/inc/HttpHeader.h"
#include "protocol/http/inc/HttpHeaderAnalyzer.h"

// TODO: do we need a simple factory to capsulate decoder initialization?
//#include "protocol/http/inc/UcHttpChunkedDecoder.h"
//#include "protocol/http/inc/UcHttpHeaderAUTO.h"
// #include "http/inc/UcCookieMgrProcess.h"
//#include "protocol/http/inc/UcDocTypeScanner.h"
//static const char *HEX = "0123456789ABCDEF";

#if 0
bool isHttpLineBreak(char* sBuf, int nLength)
{
    return (nLength >=2 ) && ('\r' == sBuf[0]) && ('\n' == sBuf[1]);
}
#endif

////////////////////////////////////////////////////////////////////////////////
//
HttpHeaderAnalyzer_c::HttpHeaderAnalyzer_c() :
    eMsgType_m(HTTP_REQUEST),
    nStatusCode_m(HTTP_RESP_CODE_UNKNOWN),
    bIsHeaderReady_m(false),
    lstHeaderInfos_m(10)
{
}

HttpHeaderAnalyzer_c::~HttpHeaderAnalyzer_c()
{
    clearHeaderInfo();
}

void HttpHeaderAnalyzer_c::releaseDecoder()
{
    if (NULL != pDecoder_m)
    {
        delete pDecoder_m;
        pDecoder_m = NULL;
    }
}

void HttpHeaderAnalyzer_c::reset()
{
    nStatusCode_m   = HTTP_RESP_CODE_UNKNOWN;
    bIsHeaderReady_m= false;
    nBodyLength_m   = 0;

    clearHeaderInfo();

    releaseDecoder();
}

void HttpHeaderAnalyzer_c::disableHeaderParse()
{
    nStatusCode_m    = HTTP_RESP_CODE_200;
    bIsHeaderReady_m = true;
}

bool HttpHeaderAnalyzer_c::setHttpDecoder()
{
#if 0
    static const char sChunked[] = "chunked";
    static const char sGZip   [] = "gzip";
    static const char sXGZip  [] = "x-gzip";
    static const char sText   [] = "text";
#endif

#if 0
    if (isNeedDocTypeScanner())
    {
        UcProtocolDecoder_c* newDecoder = new UcDocTypeScanner_c(pDecoder_m);
        if (newDecoder)
        {
            pDecoder_m = newDecoder;
        }
        else
        {
            return false;
        }
    }

    if (isNeedCharsetDecoder())
    {
        const  char * sContentType = getAttribute(HTTP_HEADER_CONTENT_TYPE);

        if(NULL != sContentType)
        {
            int sContentTypeLen = uc_strlen(sContentType);
            if(NULL != xstrnstri(sContentType,sText,sContentTypeLen)
                    || xstrnstri(sContentType, "html", sContentTypeLen)
                    || xstrnstri(sContentType, "xhtml", sContentTypeLen)
                    || xstrnstri(sContentType, "wml", sContentTypeLen)
                    )
            {
                UcProtocolDecoder_c * newDecoder = new UcCharsetDecoder_c(pDecoder_m);
                if(newDecoder != NULL)
                {
                    pDecoder_m = newDecoder;
                }
                else
                {
                    return UC_FALSE;
                }
            }
        }
    }
#endif

#if 0
    const char* sContentEncoding    = getAttribute(HTTP_HEADER_CONTENT_ENCODING);
    if ((NULL != sContentEncoding)
            && ((0 == xstrnicmp((const char*)sContentEncoding, (const char*)sGZip, xstrlen(sGZip) - 1))
                || (0 == xstrnicmp((const char*)sContentEncoding, (const char*)sXGZip, xstrlen(sXGZip) - 1 ))))
    {
        ProtocolDecoder_i * newDecoder;

        newDecoder = new UcHttpGZipDecoder_c(pDecoder_m);
        if(newDecoder != NULL)
        {
            pDecoder_m = newDecoder;
        }
        else
        {
            return false;
        }
    }

    const char* sTransferEncoding   = getAttribute(HTTP_HEADER_TRANSFER_ENCODING);
    if ((NULL != sTransferEncoding)
            && (0 == xstrnicmp((const char*)sTransferEncoding,(const char*) sChunked, sizeof(sChunked) - 1 )))
    {
        ProtocolDecoder_i * newDecoder;

        newDecoder = new HttpChunkedDecoder_c(pDecoder_m);
        if(newDecoder != NULL)
        {
            pDecoder_m = newDecoder;
        }
        else
        {
            return false;
        }
    }
#endif

    return true;
}

const char* HttpHeaderAnalyzer_c::getAttribute(int nType) const
{
    for (int nIndex=0; nIndex < lstHeaderInfos_m.size(); nIndex++)
    {
        if (nType == lstHeaderInfos_m.get(nIndex).nHeaderIndex_m)
        {
            return lstHeaderInfos_m.get(nIndex).sValue_m;
        }
    }

    return ProtocolDecoder_i::getAttribute(nType);
}

int HttpHeaderAnalyzer_c::getContentLength()
{
    const char* sContentLength = getAttribute(HTTP_HEADER_CONTENT_LENGTH);
    if (NULL != sContentLength)
    {
        return xatoi(sContentLength);
    }

    return 0;
}

DecodeRetStatus_e HttpHeaderAnalyzer_c::decode(char* sBuf, int nLen, char** sOutBuf, int& nOutLen)
{
    DecodeRetStatus_e ret = DECODE_STATUS_UNKNOWN;

    if ((NULL == sBuf) || (0 == nLen))
    {
        return ret;
    }

    *sOutBuf    = NULL;
    nOutLen     = 0;

    int nBodyStartPos = 0;

    // if header is not analyzed yet
    if (!bIsHeaderReady_m)
    {
        // header temporary buffer is not allocated, that means this is the very first
        // package from server, check whether the whole header has been ready, if so, we 
        // can analyze on request buffer rather than allocate a new buffer
        if (0 == mbHeaderBuffer_m.getLength())
        {
            int nEndPos = -1;
            for (int nIndex=0; nIndex<(nLen-3); nIndex++)
            {
                if (('\r' == sBuf[nIndex])
                        && ((nIndex + 3) < nLen)
                        && ('\n' == sBuf[nIndex+1])
                        && ('\r' == sBuf[nIndex+2])
                        && ('\n' == sBuf[nIndex+3]))
                {
                    nEndPos = nIndex + 3;

                    nBodyStartPos = nEndPos + 1;
                    break;
                }
            }

            // header end flag is found, analyze header, else push data into buffer
            if (-1 != nEndPos)
            {
                // analyze header info
                parseHeader(sBuf, nEndPos + 1);

                // handle header
                //Check the cookies. if has set cookies in header, forward to cookies mgr.              
                if(setHttpDecoder())
                {
                    bIsHeaderReady_m = true;
                    ret = DECODE_STATUS_HEADER_COMPLETE;
                }
                else
                {
                    ret = DECODE_STATUS_FAILURE;
                }

            }
            else
            {
                ret = DECODE_STATUS_HEADER_CONTINUE;
            }
        }

        if (!bIsHeaderReady_m)// && (NULL != sHeaderBuffer_m))
        {
            ret = DECODE_STATUS_HEADER_CONTINUE;

            // check if need to resize header buffer
            /*
            if (nContentLen_m + nLen > nBufferLen_m)
            {
                UC_INT32 nOrigSize = nBufferLen_m;

                // adjust buffer length
                do
                {
                    nBufferLen_m <<= 1;
                } while (nBufferLen_m < nContentLen_m + nLen);
                

                sHeaderBuffer_m = (char*)TR_MEMORY_REALLOC(sHeaderBuffer_m, nOrigSize, nBufferLen_m);
                if (NULL == sHeaderBuffer_m)
                {
                    // memory error
                    return DECODE_STATUS_UNKNOWN;
                }

                //MSG("adjust buffer length to %d\n", nBufferLen_m);
            }
            */

            // append data into header buffer until:
            //  1) end flag is encountered, or
            //  2) to the end of input
            for (int nPos=0; nPos<nLen; nPos++)
            {
                mbHeaderBuffer_m.append(sBuf[nPos]);

                // check if last four characters is header end flag: "\r\n\r\n"
                char* pHeaderContent    = mbHeaderBuffer_m.getBuffer();
                int nHeaderLength       = mbHeaderBuffer_m.getLength();
                if ((nHeaderLength >= 4)
                        && ('\r' == pHeaderContent[nHeaderLength-4])
                        && ('\n' == pHeaderContent[nHeaderLength-3])
                        && ('\r' == pHeaderContent[nHeaderLength-2])
                        && ('\n' == pHeaderContent[nHeaderLength-1]))
                {
                    // analyze Header Info
                    parseHeader(pHeaderContent, nHeaderLength);

                    // handle header
                    if(setHttpDecoder())
                    {
                        nBodyStartPos   = nPos + 1;
                        bIsHeaderReady_m= true;
                        ret             = DECODE_STATUS_HEADER_COMPLETE;
                    }
                    else
                    {
                        ret = DECODE_STATUS_FAILURE;
                    }

                    break;
                }
            }
        }
    }
    else
    {
        ret = DECODE_STATUS_CONTINUE;
    }

    // header has been analyzed
    if (bIsHeaderReady_m && (nBodyStartPos < nLen))
    {
        nBodyLength_m += (nLen - nBodyStartPos);

        if (NULL == pDecoder_m)
        {
            // if no decoder, echo content back directly
            *sOutBuf = sBuf + nBodyStartPos;
            nOutLen  = nLen - nBodyStartPos;
        }
        else
        {
            // else call decoder to decode raw data
            ret = pDecoder_m->decode(sBuf + nBodyStartPos, nLen - nBodyStartPos, sOutBuf, nOutLen);
#if 0
            switch (eRet)
            {
                case DECODE_STATUS_FAILURE:
                case DECODE_STATUS_COMPLETE:
                    ret = INPUT_STREAM_RET_CODE_HTTP_BODY_COMPLETE;
                    break;
                case DECODE_STATUS_CONTINUE:
                default:
                    break;
            }
#endif
        }

        if (nBodyLength_m == getContentLength())
        {
            ret = DECODE_STATUS_COMPLETE;
        }
    }

    return ret;
}

void HttpHeaderAnalyzer_c::parseHeader(char* sBuf, int nLength)
{
    if ((NULL == sBuf) || (0 == nLength))
    {
        return;
    }

    char* pHeaderBuf = sBuf;
    int nHeaderLen   = nLength;

    // 0) filter empty line at the begining
    while ((nHeaderLen > 0) && (true == isEmptyLine(pHeaderBuf, nHeaderLen)))
    {
        pHeaderBuf += 2;
        nHeaderLen -= 2;
    }

    // 1) Request/Status line:
    switch (eMsgType_m)
    {
        case HTTP_REQUEST:
            if (false == retrieveReqMethod(pHeaderBuf, nHeaderLen))
            {
                return;
            }
            break;
        case HTTP_RESPONSE:
            //    HTTP-Version SP Status-Code SP Reason-Phrase CRLF
            if (false == retrieveStatusCode(pHeaderBuf, nHeaderLen))
            {
                return;
            }
            break;
        default:
            return;
    }
    
    // 2) analyze header and append attribute pairs into list
    while (true == jumpToNextLine(&pHeaderBuf, nHeaderLen))
    {
        if (true == isEmptyLine(pHeaderBuf, nHeaderLen))
        {
            // To the end of header, it's the time to leave
            return;
        }

        retrieveAttribute(pHeaderBuf, nHeaderLen);
    }

}

bool HttpHeaderAnalyzer_c::retrieveReqMethod(char* sBuf, int nLength)
{
    // TODO: to get request method when needed
    return jumpToNextLine(&sBuf, nLength);
}

bool HttpHeaderAnalyzer_c::retrieveStatusCode(char* sBuf, int nLength)
{
    for (int nPos=0; nPos<nLength; nPos++)
    {
        // find the first space
        switch(sBuf[nPos])
        {
            case ' ':
                // the status code should be starting from here
                {
                    nStatusCode_m = (HttpResponseCode_e)xatoi(sBuf + nPos + 1);
                }
                return true;
            case '\r':
            case '\n':
                // line break, no status code here???
                {
                }
                return false;
            default:;
        }
    }

    return false;
}

bool HttpHeaderAnalyzer_c::retrieveAttribute(char* sBuf, int nLength)
{
    // filter the space
    if (false == jumpToNonSpace(&sBuf, nLength))
    {
        return false;
    }

    //HttpAttrInfo_t headerItem;

    char* sValue        = NULL;
    int nValueLen       = 0;

    //find the length
    int nHeaderInfoLen  = 0;
    for (;;)
    {
        char curChar = *(sBuf + nHeaderInfoLen);
        if (curChar == ' ' || curChar == ':' || curChar == '\t')
        {
             break;
        }
        nHeaderInfoLen++;
    }
    
#if 0
    UcHttpHeaderKeyWord*  pKeyWord = findHttpHeader(sBuf,nHeaderInfoLen);
    if (NULL == pKeyWord)
    {
        return false;
    }
#endif
    int nAttrIndex = getHeaderIndex(sBuf, nLength);
    if (-1 == nAttrIndex)
    {
        // unknown header
        return false;
    }

    //headerItem.nHeaderIndex_m = pKeyWord->nToken_m;
    getHeaderValue(sBuf, nLength, &sValue, nValueLen);

    if ((NULL != sValue) && (nValueLen > 0))
    {
        bool bAttrReplaced = false;
#if 0
        // from rfc 2109
        // An origin server MAY include multiple Set-Cookie headers in a response.
        // so first we try to update the Set-Cookie attribute as long as it exists.      
        // for multiple cookies, we use unsigned charactor 255 to seperate.
        // handle cookie replacement.
        if (HTTP_HEADER_SET_COOKIE == pKeyWord->nToken_m)
        {
            for (int nIndex=0; nIndex < lstHeaderInfos_m.size(); nIndex++)
            {
                if (HTTP_HEADER_SET_COOKIE ==  lstHeaderInfos_m.get(nIndex).nHeaderIndex_m)
                {
                    UcHttpResponseHeaderInfo_t *pCookieHeader = lstHeaderInfos_m.getRef(nIndex);
                    UC_CHAR *sOrigCookie = pCookieHeader->sValue_m;
                    UC_INT nOrigCookieLen = uc_strlen(sOrigCookie);
                    pCookieHeader->sValue_m = (UC_CHAR *)UC_TR_MEMORY_REALLOC(sOrigCookie, nOrigCookieLen+1, nOrigCookieLen + nValueLen + 3);                
                   
                    pCookieHeader->sValue_m[nOrigCookieLen] = 0xff;
                    uc_memcpy((pCookieHeader->sValue_m + nOrigCookieLen + 1), sValue, nValueLen);
                    pCookieHeader->sValue_m[nOrigCookieLen + nValueLen + 1] = '\0';

                    bAttrReplaced = UC_TRUE;
                    break;
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////////////
        //  Content-Range = "Content-Range" ":" content-range-spec
        //  content-range-spec      = byte-content-range-spec
        //  byte-content-range-spec = bytes-unit SP
        //                            byte-range-resp-spec "/"
        //                            ( instance-length | "*" )
        //  byte-range-resp-spec = (first-byte-pos "-" last-byte-pos)
        //                                 | "*"
        //  instance-length           = 1*DIGIT
        //////////////////////////////////////////////////////////////////////////////////
        // handle content-range replacement.
        if(HTTP_HEADER_CONTENT_RANGE == pKeyWord->nToken_m)
        {
            const UC_CHAR* units = "bytes ";
            UC_INT32 nUnitsLen = uc_strlen(units);

            UC_CHAR* sContentRageValue = sValue + nUnitsLen;
            UC_INT32 nLen = nValueLen - nUnitsLen;

            if(!trimBeginToDigit(sContentRageValue, nLen))
            {
                nStatusCode_m = HTTP_RESP_CODE_416;
            }

            // find '-'.
            UC_CHAR* sEnd = uc_strchr(sContentRageValue, '-');
            if(nLen > 0 && NULL != sEnd)
            {
                appendHeaderItem(sContentRageValue, sEnd - sContentRageValue, HTTP_HEADER_DL_UP_CONTENT_BEGIN_POS);
                nLen -= sEnd - sContentRageValue;
                sContentRageValue = sEnd;

                if(!trimBeginToDigit(sContentRageValue, nLen))
                {
                    nStatusCode_m = HTTP_RESP_CODE_416;
                }
            }

            // fine '/'.
            sEnd = uc_strchr(sContentRageValue, '/');
            if(nLen > 0 && NULL != sEnd)
            {
                appendHeaderItem(sContentRageValue, sEnd - sContentRageValue, HTTP_HEADER_DL_UP_CONTENT_END_POS);
                nLen -= sEnd - sContentRageValue;
                sContentRageValue = sEnd;

                if(!trimBeginToDigit(sContentRageValue, nLen))
                {
                    nStatusCode_m = HTTP_RESP_CODE_416;
                }
            }

            // total len.
            if(nLen > 0)
            {
                appendHeaderItem(sContentRageValue, nLen, HTTP_HEADER_DL_UP_CONTENT_TOTAL_LEN);
            }

            bAttrReplaced = UC_TRUE;
        }
#endif

        if (true != bAttrReplaced)
        {
            appendHeaderItem(sValue, nValueLen, nAttrIndex);
            //headerItem.sValue_m = (char *)UC_TR_MEMORY_ALLOC(nValueLen + 1);
            //if (NULL != headerItem.sValue_m)
            //{
            //    uc_memcpy(headerItem.sValue_m, sValue, nValueLen);
            //    headerItem.sValue_m[nValueLen] = 0;

            //    // append header to list
            //    lstHeaderInfos_m.append(headerItem);
            //}
        }
    }

    return true;
}

bool HttpHeaderAnalyzer_c::trimBeginToDigit(char*& sValue, int& nLen)
{
    while(!x_isdigit(sValue[0]))
    {
        sValue++;
        nLen--;

        if(nLen <= 0)
        {
            return false;
        }
    }

    return true;
}

void HttpHeaderAnalyzer_c::appendHeaderItem(char* sValue, int nLength, int keyIndex)
{
    HttpAttrInfo_t headerItem;

    headerItem.nHeaderIndex_m = keyIndex;
    headerItem.sValue_m = (char *)TR_MEMORY_ALLOC(nLength + 1);
    if (NULL != headerItem.sValue_m)
    {
        xmemcpy(headerItem.sValue_m, sValue, nLength);
        headerItem.sValue_m[nLength] = 0;

        // append header to list
        lstHeaderInfos_m.append(headerItem);
    }
}

void HttpHeaderAnalyzer_c::getHeaderValue(char* sBuf, int nLength, char** sValue, int& nValueLen)
{
    *sValue = NULL;
    nValueLen = 0;

    // find separator ':'
    if (false == jumpToCharacter(&sBuf, nLength, ':'))
    {
        return;
    }

    *sValue = sBuf + 1;
    nLength --;

    // filter following space
    if (false == jumpToNonSpace(sValue, nLength))
    {
        return;
    }

    // calculate value length
    while ((nValueLen < nLength) && (false == isEmptyLine(*sValue + nValueLen, nLength - nValueLen)))
    {
        nValueLen++;
    }
}

bool HttpHeaderAnalyzer_c::jumpToNonSpace(char** sBuf, int& nLength)
{
    if (0 >= nLength)
    {
        return false;
    }

    while ((' ' == **sBuf) || ('\t' == **sBuf))
    {
        (*sBuf)++;
        nLength--;
        if (0 >= nLength)
        {
            return false;
        }
    }

    return true;
}

bool HttpHeaderAnalyzer_c::jumpToCharacter(char** sBuf, int& nLength, char ch)
{
    if (0 >= nLength)
    {
        return false;
    }

    while (ch != **sBuf)
    {
        (*sBuf)++;
        nLength--;
        if (0 >= nLength)
        {
            return false;
        }
    }

    return true;
}

bool HttpHeaderAnalyzer_c::jumpToNextLine(char** sBuf, int& nLength)
{
    // string is not ended by 0, don't use strstr() to find sub-string
    for (;nLength > 0;)
    {
        if (false == isEmptyLine(*sBuf, nLength))
        {
            (*sBuf)++;
            nLength--;
        }
        else
        {
            (*sBuf) += 2;
            nLength -= 2;

            return true;
        }
    }

    return false;
}

bool HttpHeaderAnalyzer_c::isEmptyLine(char* sBuf, int nLength)
{
    return (nLength >=2 ) && ('\r' == sBuf[0]) && ('\n' == sBuf[1]);
}

void HttpHeaderAnalyzer_c::printHeader()
{
    //MSG("content length=%d\n%s\n", nContentLen_m, sHeaderBuffer_m);
    //if (true == bIsHeaderReady_m)
    //{
    //    MSG("---------------------\n");
    //    MSG("Response Header Info:\n"); 
    //    MSG(" status code: %d\n", nStatusCode_m); 

    //    for (int nIndex=0; nIndex<lstHeaderInfos_m.size(); nIndex++)
    //    {
    //        MSG(" %s: %s\n",
    //                httpHeaderInfo_m[lstHeaderInfos_m.get(nIndex).nHeaderIndex_m].sName_m,
    //                lstHeaderInfos_m.get(nIndex).sValue_m);
    //    }

    //    MSG("---------------------\n");
    //}
    //else
    //{
    //    MSG("Header is not ready yet\n");
    //}
}

void HttpHeaderAnalyzer_c::clearHeaderInfo()
{
    for (int nIndex=lstHeaderInfos_m.size()-1; nIndex>=0; nIndex--)
    {
        TR_MEMORY_FREE(lstHeaderInfos_m.get(nIndex).sValue_m);
    }
    
    lstHeaderInfos_m.clear();
}


