#include <ctype.h>

#include <protocol/http/inc/HttpHeader.h>

const char* UC_HTTP_PROTOCOL                            = "HTTP";
const char* UC_HTTP_PROTOCOL_VERSION                    = "1.1";

const HttpHeaderInfo_t httpHeaderInfo_m[] = 
{
    {"Accept",                  HTTP_HEADER_ACCEPT},
    {"Accept-Charset",          HTTP_HEADER_ACCEPT_CHARSET},
    {"Accept-Encoding",         HTTP_HEADER_ACCEPT_ENCODING},
    {"Accept-Language",         HTTP_HEADER_ACCEPT_LANGUAGE},
    {"Accept-Ranges",           HTTP_HEADER_ACCEPT_RANGES},
    {"Age",                     HTTP_HEADER_AGE},
    {"Allow",                   HTTP_HEADER_ALLOW},
    {"Authorization",           HTTP_HEADER_AUTHORIZATION},
    {"Cache-Control",           HTTP_HEADER_CACHE_CONTROL},
    {"Connection",              HTTP_HEADER_CONNECTION},
    {"Content-Encoding",        HTTP_HEADER_CONTENT_ENCODING},
    {"Content-Language",        HTTP_HEADER_CONTENT_LANGUAGE},
    {"Content-Length",          HTTP_HEADER_CONTENT_LENGTH},
    {"Content-Location",        HTTP_HEADER_CONTENT_LOCATION},
    {"Content-MD5",             HTTP_HEADER_CONTENT_MD5},
    {"Content-Range",           HTTP_HEADER_CONTENT_RANGE},
    {"Content-Type",            HTTP_HEADER_CONTENT_TYPE},
    {"Date",                    HTTP_HEADER_DATE},
    {"ETag",                    HTTP_HEADER_ETAG},
    {"Expect",                  HTTP_HEADER_EXPECT},
    {"Expires",                 HTTP_HEADER_EXPIRES},
    {"From",                    HTTP_HEADER_FROM},
    {"Host",                    HTTP_HEADER_HOST},
    {"If-Match",                HTTP_HEADER_IF_MATCH},
    {"If-Modified-Since",       HTTP_HEADER_IF_MODIFIED_SINCE},
    {"If-None-Match",           HTTP_HEADER_IF_NONE_MATCH},
    {"If-Range",                HTTP_HEADER_IF_RANGE},
    {"If-Unmodified-Since",     HTTP_HEADER_IF_UNMODIFIED_SINCE},
    {"Last-Modified",           HTTP_HEADER_LAST_MODIFIED},
    {"Location",                HTTP_HEADER_LOCATION},
    {"Max-Forwards",            HTTP_HEADER_MAX_FORWARDS},
    {"Pragma",                  HTTP_HEADER_PRAGMA},
    {"Proxy-Authenticate",      HTTP_HEADER_PROXY_AUTHENTICATE},
    {"Proxy-Authorization",     HTTP_HEADER_PROXY_AUTHORIZATION},
    {"Range",                   HTTP_HEADER_RANGE},
    {"Referer",                 HTTP_HEADER_REFERER},
    {"Retry-After",             HTTP_HEADER_RETRY_AFTER},
    {"Server",                  HTTP_HEADER_SERVER},
    {"TE",                      HTTP_HEADER_TE},
    {"Trailer",                 HTTP_HEADER_TRAILER},
    {"Transfer-Encoding",       HTTP_HEADER_TRANSFER_ENCODING},
    {"Upgrade",                 HTTP_HEADER_UPGRADE},
    {"User-Agent",              HTTP_HEADER_USER_AGENT},
    {"Vary",                    HTTP_HEADER_VARY},
    {"Via",                     HTTP_HEADER_VIA},
    {"Warning",                 HTTP_HEADER_WARNING},
    {"WWW-Authenticate",        HTTP_HEADER_WWW_AUTHENTICATE},
};

#define N_HTTP_HEADERS (sizeof(httpHeaderInfo_m)/sizeof(httpHeaderInfo_m[0]))

//
// Compares header from buffer (':' or space-ended string) [p1]
// with tag from header list (zero ended string) [p2]
// Return value: as strcmp()
//
int compareHeaderIc(const char *p1, int nLen, const char *p2)
{
    while ((*p2) && (nLen > 0))
    {
        if (tolower(*p1) != tolower(*p2))
        {
            return(tolower(*p1) - tolower(*p2));
        }
        ++p1;
        ++p2;

        nLen--;
    }

    if ((0 == *p2)
            && ((0 == nLen) || (' ' == *p1) || (':' == *p1) || ('\t' == *p1)))
    {
        return 0;
    }
    else if (0 == nLen)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

//
// Get http header index
// return -1 if it is not handled yet
//
int getHeaderIndex(const char *header, int nLen)
{
    int low  = 0;
    int high = N_HTTP_HEADERS - 1;    // Last header index 
    int mid;
    int cond;

    // Binary search 
    while (low <= high)
    {
        mid = ((low + high) >> 1);
        if ((cond = compareHeaderIc(header, nLen, httpHeaderInfo_m[mid].sName_m)) < 0)
        {
            high = mid - 1;
        }
        else if (cond > 0)
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }

    return -1;
}

const char* getHeaderDescription(int nIndex)
{
    if ((nIndex < 0) || ((unsigned int)nIndex >= N_HTTP_HEADERS))
    {
        return (const char*)0;
    }

    return httpHeaderInfo_m[nIndex].sName_m;
}
                       
/*
// RFC2616 chapter 4.5: Http general header
const char* UC_HTTP_HEADER_CACHE_CONTROL                = "Cache-Control";
const char* UC_HTTP_HEADER_CONNECTION                   = "Connection";
const char* UC_HTTP_HEADER_DATE                         = "Date";
const char* UC_HTTP_HEADER_PRAGMA                       = "Pragma";
const char* UC_HTTP_HEADER_TRAILER                      = "Trailer";
const char* UC_HTTP_HEADER_TRANSFER_ENCODING            = "Transfer-Encoding";
const char* UC_HTTP_HEADER_UPGRADE                      = "Upgrade";
const char* UC_HTTP_HEADER_VIA                          = "Via";
const char* UC_HTTP_HEADER_WARNING                      = "Warning";

// RFC2616 chapter 5.3: request header
const char* UC_HTTP_REQUEST_HEADER_ACCEPT               = "Accept";
const char* UC_HTTP_REQUEST_HEADER_ACCEPT_CHARSET       = "Accept-Charset";
const char* UC_HTTP_REQUEST_HEADER_ACCEPT_ENCODING      = "Accept-Encoding";
const char* UC_HTTP_REQUEST_HEADER_ACCEPT_LANGUAGE      = "Accept-Language";
const char* UC_HTTP_REQUEST_HEADER_AUTHORIZATION        = "Authorization";
const char* UC_HTTP_REQUEST_HEADER_EXPECT               = "Expect";
const char* UC_HTTP_REQUEST_HEADER_FROM                 = "From";
const char* UC_HTTP_REQUEST_HEADER_HOST                 = "Host";
const char* UC_HTTP_REQUEST_HEADER_IF_MATCH             = "If-Match";
const char* UC_HTTP_REQUEST_HEADER_IF_MODIFIED_SINCE    = "If-Modified-Since";
const char* UC_HTTP_REQUEST_HEADER_IF_NONE_MATCH        = "If-None-Match";
const char* UC_HTTP_REQUEST_HEADER_IF_RANGE             = "If-Range";
const char* UC_HTTP_REQUEST_HEADER_IF_UNMODIFIED_SINCE  = "If-Unmodified-Since";
const char* UC_HTTP_REQUEST_HEADER_MAX_FORWARDS         = "Max-Forwards";
const char* UC_HTTP_REQUEST_HEADER_PROXY_AUTHORIZATION  = "Proxy-Authorization";
const char* UC_HTTP_REQUEST_HEADER_RANGE                = "Range";
const char* UC_HTTP_REQUEST_HEADER_REFERER              = "Referer";
const char* UC_HTTP_REQUEST_HEADER_TE                   = "TE";
const char* UC_HTTP_REQUEST_HEADER_USER_AGENT           = "User-Agent";

// RFC2616 chapter 6.2: response header
const char* UC_HTTP_RESPONSE_HEADER_ACCEPT_RANGES       = "Accept-Ranges";
const char* UC_HTTP_RESPONSE_HEADER_AGE                 = "Age";
const char* UC_HTTP_RESPONSE_HEADER_ETAG                = "ETag";
const char* UC_HTTP_RESPONSE_HEADER_LOCATION            = "Location";
const char* UC_HTTP_RESPONSE_HEADER_PROXY_AUTHENTICATE  = "Proxy-Authenticate";
const char* UC_HTTP_RESPONSE_HEADER_RETRY_AFTER         = "Retry-After";
const char* UC_HTTP_RESPONSE_HEADER_SERVER              = "Server";
const char* UC_HTTP_RESPONSE_HEADER_VARY                = "Vary";
const char* UC_HTTP_RESPONSE_HEADER_WWW_AUTHENTICATE    = "WWW-Authenticate";

// RFC2616 chapter 7.1: entity header
const char* UC_HTTP_ENTITY_HEADER_ALLOW                 = "Allow";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_ENCODING      = "Content-Encoding";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_LANGUAGE      = "Content-Language";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_LENGTH        = "Content-Length";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_LOCATION      = "Content-Location";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_MD5           = "Content-MD5";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_RANGE         = "Content-Range";
const char* UC_HTTP_ENTITY_HEADER_CONTENT_TYPE          = "Content-Type";
const char* UC_HTTP_ENTITY_HEADER_EXPIRES               = "Expires";
const char* UC_HTTP_ENTITY_HEADER_LAST_MODIFIED         = "Last-Modified";
*/

