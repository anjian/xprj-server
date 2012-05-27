#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>
//#include <common/inc/UcStringHelper.h>

#include <interface/baseos/TransientMemory.h>
//#include <interface/system/UcSystemInfoIf.h>
//#include <interface/protocol/UcUri.h> 

#include "protocol/http/inc/HttpHeader.h"
#include "protocol/http/inc/HttpResponseGenerator.h"

// #include <protocol/http/inc/UcCookieMgrProcess.h>

//char sUcweb_m[]             = "ucweb-squid";
//char sVersion_m[]           = "8.0";
//char sCustomUserAgent_m[]   = "BREW-Applet/0x20068888 (BREW/3.1.5.20; DeviceId: 600; Lang: zhcn) ucweb-squid";           // for test

//char sCustomUserAgent_m[]   = "Opera/9.80 (Windows NT 5.1; U; zh-cn) Presto/2.7.62 Version/11.01 ";           // for test
////////////////////////////////////////////////////////////////////////////////
// The pHttpReqMethod_m MUST be exactly mapping to the Method type definition:
//
//  UcHttpRequestMethod_e in header file
////////////////////////////////////////////////////////////////////////////////
#if 0
const UcHttpRequestMethodInfo_t pHttpReqMethod_m[] =
{
    {HTTP_REQ_METHOD_GET,    "GET"},
    {HTTP_REQ_METHOD_HEADER, "HEADER"},
    {HTTP_REQ_METHOD_POST,   "POST"},
    {HTTP_REQ_METHOD_END,    ""},
};
#endif

////////////////////////////////////////////////////////////////////////////////
// The pHttpReqHandler_m MUST be exactly mapping to the step type definition:
//
//  UcHttpRequestStep_e in header file
////////////////////////////////////////////////////////////////////////////////
HttpResponseGenerator_c::HttpResponseHandlerTable_t HttpResponseGenerator_c::pHttpReqHandler_m[] =
{
    {
        //HTTP_RESPONSE_STEP_STATUS_LINE,
        &HttpResponseGenerator_c::getStatusLineEstimateSize,
        &HttpResponseGenerator_c::statusLineHandler
    },
    {
        //HTTP_RESPONSE_STEP_CONNECTION,
        &HttpResponseGenerator_c::getConnectionEstimateSize,
        &HttpResponseGenerator_c::connectionHandler
    },
    {
        //HTTP_RESPONSE_STEP_SERVER,
        &HttpResponseGenerator_c::getServerEstimateSize,
        &HttpResponseGenerator_c::serverHandler
    },
    {
        //HTTP_RESPONSE_STEP_CONTENT_TYPE,
        &HttpResponseGenerator_c::getContentTypeEstimateSize,
        &HttpResponseGenerator_c::contentTypeHandler
    },
    {
        //HTTP_RESPONSE_STEP_CONTENT_LENGTH,
        &HttpResponseGenerator_c::getContentLengthEstimateSize,
        &HttpResponseGenerator_c::contentLengthHandler
    },
    {
        //HTTP_RESPONSE_STEP_EMPTY_LINE,
        &HttpResponseGenerator_c::getEmptyLineSize,
        &HttpResponseGenerator_c::emptyLineHandler
    },
    {
        //HTTP_RESPONSE_STEP_END,
        NULL,
        NULL
    },
};

HttpResponseGenerator_c::HttpResponseGenerator_c() :
    eCurStep_m(HTTP_RESPONSE_STEP_START),
    nContentLength_m(0)
{
}

HttpResponseGenerator_c::~HttpResponseGenerator_c()
{
}

void HttpResponseGenerator_c::reset()
{
    eCurStep_m  = HTTP_RESPONSE_STEP_START;
}

OutputStreamRetCode_e HttpResponseGenerator_c::generate(char* sBuf, const int nBufLen, int& nContentLen)
{
    while ((HTTP_RESPONSE_STEP_START <= eCurStep_m) && (eCurStep_m < HTTP_RESPONSE_STEP_END))
    {
        switch (stepHandlerFrame(sBuf, nBufLen, nContentLen))
        {
            case HTTP_RSP_ITEM_STATUS_DONE:
                {
                    break;
                }
            case HTTP_RSP_ITEM_STATUS_COMPLETE:
                {
                    return OUTPUT_GEN_COMPLETE;
                }
            case HTTP_RSP_ITEM_STATUS_BUF_NOT_ENOUGH:
                {
                    // left buffer is not big enough, inform consumer to handle part of content first
                    return OUTPUT_GEN_CONTINUE;
                }
            case HTTP_RSP_ITEM_STATUS_BUF_TOO_SHORT:
                {
                    // ask consumer to give a bigger memory block
                    return OUTPUT_GEN_BUF_NOT_ENOUGH;
                }
            default:
                {
                    // it's impossible unless a error case
                    return OUTPUT_GEN_COMPLETE;
                }
        }
    }

    return OUTPUT_GEN_COMPLETE;
}

HttpResponseItemStatus_e HttpResponseGenerator_c::stepHandlerFrame(char* sBuf, const int nBufLen, int& nContentLen)
{
    //
    // 0. get estimate size
    int nEstimateSize = (this->*(pHttpReqHandler_m[eCurStep_m].getEstimateSize))();

    // 1. check buffer size
    if (nEstimateSize > nBufLen)
    {
        return HTTP_RSP_ITEM_STATUS_BUF_TOO_SHORT;
    }
    else if (nEstimateSize > (nBufLen - nContentLen))
    {
        return HTTP_RSP_ITEM_STATUS_BUF_NOT_ENOUGH;
    }
    else if (0 == nEstimateSize)
    {
        // skip empty item
        stepCompleted();
        return HTTP_RSP_ITEM_STATUS_DONE;
    }

    // 2. allocate transient memory
    char* pTrRequestLine = (char*)TR_MEMORY_ALLOC(nEstimateSize);
    if (NULL == pTrRequestLine)
    {
        // TODO: memory error handling
        return HTTP_RSP_ITEM_STATUS_DONE;
    }

    // 3. organize request line
    (this->*(pHttpReqHandler_m[eCurStep_m].stepHandler))(pTrRequestLine);

    // 4. append request line
    appendBuffer(sBuf, pTrRequestLine, nBufLen, nContentLen);

    // 5. release transient memory
    TR_MEMORY_FREE(pTrRequestLine);

    // done
    stepCompleted();
    return HTTP_RSP_ITEM_STATUS_DONE;
}

void HttpResponseGenerator_c::appendBuffer(char* sBuf, char* sPart, const int nBufLen, int& nContentLen)
{
    int nPartLen = xstrlen(sPart);

    // don't need end zero
    xmemcpy(sBuf + nContentLen, sPart, nPartLen);
    nContentLen += nPartLen;
}

#if 0
////////////////////////////////////////////////////////////////////////////////
// NOTE: internal function, pUri_m MUST be checked before calling it
////////////////////////////////////////////////////////////////////////////////
void HttpResponseGenerator_c::assembleHost(char* sBuf, UcUri_c* pUri, bool bWithScheme)
{
    if (true == bWithScheme)
    {
        if (NULL != pUri->getScheme())
        {
            uc_strcat(sBuf, pUri->getScheme());
            uc_strcat(sBuf, "://");
        }
    }

    if (NULL != pUri->getHost())
    {
        uc_strcat(sBuf, pUri->getHost());
    }

    if (NULL != pUri->getPort())
    {
        uc_strcat(sBuf, ":");
        uc_strcat(sBuf, pUri->getPort());
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
// HTTP response header 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// RFC2616 Chapter 6 (extracted)
//
//  Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
//
////////////////////////////////////////////////////////////////////////////////
static const char* HTTP_STATUS_LINE = "HTTP/1.1 200 OK";

int HttpResponseGenerator_c::getStatusLineEstimateSize()
{
    // length of status line + CRLF
    return xstrlen(HTTP_STATUS_LINE) + 4;
}

void HttpResponseGenerator_c::statusLineHandler(char* sBuf)
{
    // assemble request line
    //
    // 0) init
    *sBuf = 0;

    // 1) status line
    xstrcat(sBuf, HTTP_STATUS_LINE);

    // 4 EOL
    xstrcat(sBuf, HTTP_END_OF_LINE);

    return;
}

////////////////////////////////////////////////////////////////////////////////
// server info : RFC2616 Chapter 14.38
//
//  Server = "Server" ":" 1*( product | comment )
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getServerEstimateSize()
{
    return xstrlen(HTTP_SERVER_DEF) + 12;
}

void HttpResponseGenerator_c::serverHandler(char* sBuf)
{
    // host info
    *sBuf = 0;

    xsprintf(sBuf, "%s: %s\r\n", getHeaderDescription(HTTP_HEADER_SERVER), HTTP_SERVER_DEF);

    return;
}

////////////////////////////////////////////////////////////////////////////////
// Connection info
//
// RFC2616 chapter 8.1.2:
// persistent connections are the default behavior of any HTTP connection. That
// is, unless otherwise indicated, the client SHOULD assume that the server will
// maintain a persistent connection, even after error responses from the server
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getConnectionEstimateSize()
{
#if 0
    int nHttpPersistentConnectionEnabled = SYS_INFO_GET_INT(SYS_INFO_TYPE_PERSISTENT_CONNECTION);

    // "Connection: close" if presistent connection is not enabled
    if (true == nUcHttpPersistentConnectionEnabled)
    {
        return 0;
    }
#endif

    return xstrlen(getHeaderDescription(HTTP_HEADER_CONNECTION)) + xstrlen(HTTP_CONNECTION_CLOSE) + 6;
}

void HttpResponseGenerator_c::connectionHandler(char* sBuf)
{
    // connection
    *sBuf = 0;

#if 0
    int nHttpPersistentConnectionEnabled = UC_SYS_INFO_GET_INT(SYS_INFO_TYPE_PERSISTENT_CONNECTION);
    if (false == nHttpPersistentConnectionEnabled)
#endif
    {
        xsprintf(sBuf, "%s: %s\r\n", getHeaderDescription(HTTP_HEADER_CONNECTION), HTTP_CONNECTION_CLOSE);
    }

    return;
}


#if 0
////////////////////////////////////////////////////////////////////////////////
// Accept-charset
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getAcceptCharsetSize()
{
    return uc_strlen(STR_DEF_HTTP_HEADER_ACCEPT_CHARSET) + uc_strlen(UC_HTTP_ACCEPT_CHARSET_VALUE) + 6;
}

void HttpResponseGenerator_c::acceptCharsetHandler(char* sBuf)
{
    // Accept-Charset: utf-8,*;q=0.8\r\n
    *sBuf = 0;

    uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_ACCEPT_CHARSET, UC_HTTP_ACCEPT_CHARSET_VALUE);
}

////////////////////////////////////////////////////////////////////////////////
// Accept-Encoding
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getAcceptEncodingSize()
{
    //return 0;     // disable accept encoding
    return uc_strlen(STR_DEF_HTTP_HEADER_ACCEPT_ENCODING) + uc_strlen(UC_HTTP_ACCEPT_ENCODING_VALUE) + 6;
}

void HttpResponseGenerator_c::acceptEncodingHandler(char* sBuf)
{
    // Accept-Encoding: gzip\r\n
    *sBuf = 0;

    uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_ACCEPT_ENCODING, UC_HTTP_ACCEPT_ENCODING_VALUE);
}

////////////////////////////////////////////////////////////////////////////////
// Accept-Language
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getAcceptLanguageSize()
{
   return uc_strlen(STR_DEF_HTTP_HEADER_ACCEPT_LANGUAGE) + uc_strlen(UC_HTTP_ACCEPT_LANGUAGE_VALUE) + 6;
}

void HttpResponseGenerator_c::acceptLanguageHandler(char* sBuf)
{
    *sBuf = 0;
    uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_ACCEPT_LANGUAGE, UC_HTTP_ACCEPT_LANGUAGE_VALUE);
}

////////////////////////////////////////////////////////////////////////////////
// Accept
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getAcceptSize()
{
    int len = uc_strlen(STR_DEF_HTTP_HEADER_ACCEPT) + 6;

    switch(nAcceptType_m)
    {
        case HTTP_REQ_ACCEPT_DOWNLOAD:
            len += uc_strlen(UC_HTTP_ACCEPT_VALUE_DOWNLOAD);
            break;

        case HTTP_REQ_ACCEPT_NORMAL:
        default:
            len += uc_strlen(UC_HTTP_ACCEPT_VALUE_NORMAL);
            break;
    };

    return len;
}

void HttpResponseGenerator_c::acceptHandler(char* sBuf)
{
    *sBuf = 0;

    switch(nAcceptType_m)
    {
        case HTTP_REQ_ACCEPT_DOWNLOAD:
            uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_ACCEPT, UC_HTTP_ACCEPT_VALUE_DOWNLOAD);
            break;

        case HTTP_REQ_ACCEPT_NORMAL:
        default:
            uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_ACCEPT, UC_HTTP_ACCEPT_VALUE_NORMAL);
            break;
    };
}

////////////////////////////////////////////////////////////////////////////////
// User-Agent
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getUserAgentEstimateSize()
{
    const UC_CHAR* sUserAgent = UC_SYS_INFO_GET_STRING(SYS_INFO_TYPE_USER_AGENT);
    if (NULL == sUserAgent)
    {
        sUserAgent = sCustomUserAgent_m;
    }

    return uc_strlen(STR_DEF_HTTP_HEADER_USER_AGENT)
        + uc_strlen(sUserAgent)
        //+ uc_strlen(sUcweb_m)
       // + uc_strlen(sVersion_m)
        //+ 8;        // for ':', ' ', "/r/n"
        + 6;        // for ':', ' ', "/r/n"
}

void HttpResponseGenerator_c::userAgentHandler(char* sBuf)
{
    const UC_CHAR* sUserAgent = UC_SYS_INFO_GET_STRING(SYS_INFO_TYPE_USER_AGENT);
    if (NULL == sUserAgent)
    {
        sUserAgent = sCustomUserAgent_m;
    }

    // User-Agent
    *sBuf = 0;

    uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_USER_AGENT, sUserAgent);

    //uc_strcpy(sBuf, STR_DEF_HTTP_HEADER_USER_AGENT);
    //uc_strcat(sBuf, ":");
    //uc_strcat(sBuf, UC_HTTP_SPACE);

    //// if uc_strlen(sCustomUserAgent_m) > 0
    //if (0 != sUserAgent[0])
    //{
    //    uc_strcat(sBuf, sUserAgent);
    //    uc_strcat(sBuf, UC_HTTP_SPACE);
    //}

    //// product/version
    ////uc_strcat(sBuf, sUcweb_m);
    ////uc_strcat(sBuf, "/");
    ////uc_strcat(sBuf, sVersion_m);

    //// end of line
    //uc_strcat(sBuf, UC_HTTP_END_OF_LINE);
}

////////////////////////////////////////////////////////////////////////////////
// Range
//
// NOTE: Do not support the range beyond 4G
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getRangeEstimateSize()
{
    UC_INT32 len = 0;

    // should bigger enough even for 64 bits integer
    if(HTTP_REQ_FLAG_SET == bRangeSet_m)
    {
        len += uc_strlen(STR_DEF_HTTP_HEADER_RANGE)
            + 6 // "bytes="
            + 2 // ": "
            + 64;
    }

    return len;
}

void HttpResponseGenerator_c::rangeHandler(char* sBuf)
{
    *sBuf = 0;

    if (HTTP_REQ_FLAG_SET == bRangeSet_m)
    {
        uc_sprintf(sBuf, "%s: bytes=%u-%u\r\n",
                STR_DEF_HTTP_HEADER_RANGE,
                nFirstBytePos_m,
                nLastBytePos_m);

        // reset range flag
        bRangeSet_m = HTTP_REQ_FLAG_NOT_SET;
    }
}

void HttpResponseGenerator_c::setRange(UC_INT32 nFPos, UC_INT32 nLPos)
{
    bRangeSet_m     = HTTP_REQ_FLAG_SET;

    nFirstBytePos_m = nFPos;
    nLastBytePos_m  = nLPos;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Content-Type
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getContentTypeEstimateSize()
{
    return xstrlen(getHeaderDescription(HTTP_HEADER_CONTENT_TYPE)) + xstrlen(HTTP_CONTENT_TYPE_BIN) + 6; 
}

void HttpResponseGenerator_c::contentTypeHandler(char* sBuf)
{
    // init
    *sBuf = 0;
    xsprintf(sBuf, "%s: %s\r\n", getHeaderDescription(HTTP_HEADER_CONTENT_TYPE), HTTP_CONTENT_TYPE_BIN);
}

#if 0
////////////////////////////////////////////////////////////////////////////////
// Referer
//
// RFC2616 Chapter 14.36
//  .) Mainly it's for server benefit...
//  .) The Referer field MUST NOT be sent if the Request-URI was obtained from a
//     source that does not have its own URI, such as input from the user keyboard
//
//     (Controlled by outside? or check uri to see whether it has a base uri?)
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getRefererEstimateSize()
{
    int nLen = 0;

    //switch (nRefererType_m)
    //{
    //    case HTTP_REQ_REFERER_PATH:
    //        // path part
    //        nLen = pUri_m->getPathLen() + 6;

    //        // !!!NO break, skip to plus authority length
    //    case HTTP_REQ_REFERER_HOST:
    //        nLen += pUri_m->getSchemeLen()
    //            + pUri_m->getHostLen()
    //            + pUri_m->getPortLen()
    //            + 6;
    //        break;
    //    default:;
    //}

    if(NULL != pRefererUri_m)
    {
        nLen += uc_strlen(STR_DEF_HTTP_HEADER_REFERER)
            + pRefererUri_m->getSchemeLen()
            + pRefererUri_m->getHostLen()
            + pRefererUri_m->getPortLen()
            + pRefererUri_m->getPathLen()
            + pRefererUri_m->getQueryLen()
            + pRefererUri_m->getFragmentLen()
            + 9 //"/r/n", ":", ": ", "?", "#"
            + 1;
    }

    return nLen;
}

void HttpResponseGenerator_c::refererHandler(char* sBuf)
{
    *sBuf = 0;

    UC_CHAR* pUri = pRefererUri_m->generateUri(UC_TRUE);
    if (pUri)
    {
        // append header.
        uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_REFERER, pUri);
    }

    SAFE_DELETE_TR_PTR(pUri);
}
#endif

////////////////////////////////////////////////////////////////////////////////
// Content-Length
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getContentLengthEstimateSize()
{
  return (nContentLength_m > 0) ? xstrlen(getHeaderDescription(HTTP_HEADER_CONTENT_LENGTH)) + 12 : 0;
}

void HttpResponseGenerator_c::contentLengthHandler(char* sBuf)
{
    *sBuf = 0;
    if (nContentLength_m > 0)
    {
        // append header
        xsprintf(sBuf, "%s: %d\r\n", getHeaderDescription(HTTP_HEADER_CONTENT_LENGTH), nContentLength_m);
        //bContentLengthNeeded_m = HTTP_REQ_FLAG_NOT_SET;
    }

}

#if 0
////////////////////////////////////////////////////////////////////////////////
// Cookie
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getCookieSize()
{
    if (!bCookieEnabled_m || NULL == sCookieData_m)
    {
        return 0;
    }

    return (uc_strlen(sCookieData_m) + uc_strlen(STR_DEF_HTTP_HEADER_COOKIE) + 6);
}

void HttpResponseGenerator_c::cookieHandler(char* sBuf)
{
    if (!bCookieEnabled_m || NULL == sCookieData_m)
    {
        return;
    }

    *sBuf = 0;

    uc_sprintf(sBuf, "%s: %s\r\n", STR_DEF_HTTP_HEADER_COOKIE, sCookieData_m);

    ucfree(sCookieData_m);
    sCookieData_m = NULL;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// empty line '\r\n'
////////////////////////////////////////////////////////////////////////////////
int HttpResponseGenerator_c::getEmptyLineSize()
{
    // "/r/n"
    return sizeof(HTTP_END_OF_LINE) + 1;
}

void HttpResponseGenerator_c::emptyLineHandler(char* sBuf)
{
    *sBuf = 0;
    xstrcpy(sBuf, HTTP_END_OF_LINE);

    return;
}

#if 0
void HttpResponseGenerator_c::setCookies(const UC_CHAR*  sCookieData)
{
    if (NULL == sCookieData)
    {
        return;
    }

    bCookieEnabled_m = UC_TRUE;

    sCookieData_m = UcStringDuplicate(sCookieData, uc_strlen(sCookieData) + 1);
}
#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
// test
////////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[])
{
    //UcTransientMemoryPool_c* pTrPool = UcTransientMemoryPool_c::getInstance();

    MSG("\nHttp Request:\n");
    {
        UcUri_c* uriBase = UcUri_c::resolveUri("http://www.163.com/ba/bb/bc/bd/.?bq=bq#basefra", NULL);
        uriBase->print();
        MSG("\n");

        UcUri_c* uriRel = UcUri_c::resolveUri("ra?rq=rq#relfra", uriBase);
        uriRel->print();
        MSG("\n");

        UcHttpRequest_c httpReq(uriRel);
        httpReq.setMethod(HTTP_REQ_METHOD_POST);
        httpReq.setMultiPart();
        httpReq.setReferer(HTTP_REQ_REFERER_PATH);
        httpReq.setRange(0xfffffff, 0xffffffff);
        char sBuf[256];
        int nContentLen = 0;

        UcOutputStreamRetCode_e ret;

        MSG("HTTP Header:\n---------\n");
        do
        {
            ret = httpReq.generate(sBuf, 256, nContentLen);

            *(sBuf + nContentLen) = 0;
            MSG("%s", sBuf);

            nContentLen = 0;
        } while (OUTPUT_GEN_COMPLETE != ret);

        MSG("---------\n");
    }

    return 0;
}

#endif

