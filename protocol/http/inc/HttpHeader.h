#ifndef __HTTP_HEADER_H_
#define __HTTP_HEADER_H_

int getHeaderIndex(const char *header, int nLen);
const char* getHeaderDescription(int nIndex);

extern const char* HTTP_PROTOCOL;
extern const char* HTTP_PROTOCOL_VERSION;

////////////////////////////////////////////////////////////////////////////////
// Http Request Method
////////////////////////////////////////////////////////////////////////////////
#define HTTP_REQ_METHOD_GET         "GET"
#define HTTP_REQ_METHOD_HEADER      "HEADER"
#define HTTP_REQ_METHOD_POST        "POST"

// RFC2616 chapter 4.5: Http general header
// RFC2616 chapter 5.3: request header
// RFC2616 chapter 6.2: response header
// RFC2616 chapter 7.1: entity header
//
// Pleae add new type based on alphabet sequence
enum HttpHeaderType_e
{
    HTTP_HEADER_ACCEPT,
    HTTP_HEADER_ACCEPT_CHARSET,
    HTTP_HEADER_ACCEPT_ENCODING,
    HTTP_HEADER_ACCEPT_LANGUAGE,
    HTTP_HEADER_ACCEPT_RANGES,
    HTTP_HEADER_AGE,
    HTTP_HEADER_ALLOW,
    HTTP_HEADER_AUTHORIZATION,
    HTTP_HEADER_CACHE_CONTROL,
    HTTP_HEADER_CONNECTION,
    HTTP_HEADER_CONTENT_ENCODING,
    HTTP_HEADER_CONTENT_LANGUAGE,
    HTTP_HEADER_CONTENT_LENGTH,
    HTTP_HEADER_CONTENT_LOCATION,
    HTTP_HEADER_CONTENT_MD5,
    HTTP_HEADER_CONTENT_RANGE,
    HTTP_HEADER_CONTENT_TYPE,
    HTTP_HEADER_DATE,
    HTTP_HEADER_ETAG,
    HTTP_HEADER_EXPECT,
    HTTP_HEADER_EXPIRES,
    HTTP_HEADER_FROM,
    HTTP_HEADER_HOST,
    HTTP_HEADER_IF_MATCH,
    HTTP_HEADER_IF_MODIFIED_SINCE,
    HTTP_HEADER_IF_NONE_MATCH,
    HTTP_HEADER_IF_RANGE,
    HTTP_HEADER_IF_UNMODIFIED_SINCE,
    HTTP_HEADER_LAST_MODIFIED,
    HTTP_HEADER_LOCATION,
    HTTP_HEADER_MAX_FORWARDS,
    HTTP_HEADER_PRAGMA,
    HTTP_HEADER_PROXY_AUTHENTICATE,
    HTTP_HEADER_PROXY_AUTHORIZATION,
    HTTP_HEADER_RANGE,
    HTTP_HEADER_REFERER,
    HTTP_HEADER_RETRY_AFTER,
    HTTP_HEADER_SERVER,
    HTTP_HEADER_TE,
    HTTP_HEADER_TRAILER,
    HTTP_HEADER_TRANSFER_ENCODING,
    HTTP_HEADER_UPGRADE,
    HTTP_HEADER_USER_AGENT,
    HTTP_HEADER_VARY,
    HTTP_HEADER_VIA,
    HTTP_HEADER_WARNING,
    HTTP_HEADER_WWW_AUTHENTICATE,
    HTTP_HEADER_SET_COOKIE,
    HTTP_HEADER_COOKIE,

    HTTP_HEADER_END
};

struct HttpHeaderInfo_t
{
    const char* sName_m;
    HttpHeaderType_e eType_m;
};

extern const HttpHeaderInfo_t httpHeaderInfo_m[];

/*
// RFC2616 chapter 4.5: Http general header
extern const char* UC_HTTP_HEADER_CACHE_CONTROL;
extern const char* UC_HTTP_HEADER_CONNECTION;
extern const char* UC_HTTP_HEADER_DATE;
extern const char* UC_HTTP_HEADER_PRAGMA;
extern const char* UC_HTTP_HEADER_TRAILER;
extern const char* UC_HTTP_HEADER_TRANSFER_ENCODING;
extern const char* UC_HTTP_HEADER_UPGRADE;
extern const char* UC_HTTP_HEADER_VIA;
extern const char* UC_HTTP_HEADER_WARNING;

// RFC2616 chapter 7.1: entity header
extern const char* UC_HTTP_ENTITY_HEADER_ALLOW;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_ENCODING;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_LANGUAGE;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_LENGTH;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_LOCATION;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_MD5;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_RANGE;
extern const char* UC_HTTP_ENTITY_HEADER_CONTENT_TYPE;
extern const char* UC_HTTP_ENTITY_HEADER_EXPIRES;
extern const char* UC_HTTP_ENTITY_HEADER_LAST_MODIFIED;

// RFC2616 chapter 5.3: request header
extern const char* UC_HTTP_REQUEST_HEADER_ACCEPT;
extern const char* UC_HTTP_REQUEST_HEADER_ACCEPT_CHARSET;
extern const char* UC_HTTP_REQUEST_HEADER_ACCEPT_ENCODING;
extern const char* UC_HTTP_REQUEST_HEADER_ACCEPT_LANGUAGE;
extern const char* UC_HTTP_REQUEST_HEADER_AUTHORIZATION;
extern const char* UC_HTTP_REQUEST_HEADER_EXPECT;
extern const char* UC_HTTP_REQUEST_HEADER_FROM;
extern const char* UC_HTTP_REQUEST_HEADER_HOST;
extern const char* UC_HTTP_REQUEST_HEADER_IF_MATCH;
extern const char* UC_HTTP_REQUEST_HEADER_IF_MODIFIED_SINCE;
extern const char* UC_HTTP_REQUEST_HEADER_IF_NONE_MATCH;
extern const char* UC_HTTP_REQUEST_HEADER_IF_RANGE;
extern const char* UC_HTTP_REQUEST_HEADER_IF_UNMODIFIED_SINCE;
extern const char* UC_HTTP_REQUEST_HEADER_MAX_FORWARDS;
extern const char* UC_HTTP_REQUEST_HEADER_PROXY_AUTHORIZATION;
extern const char* UC_HTTP_REQUEST_HEADER_RANGE;
extern const char* UC_HTTP_REQUEST_HEADER_REFERER;
extern const char* UC_HTTP_REQUEST_HEADER_TE;
extern const char* UC_HTTP_REQUEST_HEADER_USER_AGENT;

// RFC2616 chapter 6.2: response header
extern const char* UC_HTTP_RESPONSE_HEADER_ACCEPT_RANGES;
extern const char* UC_HTTP_RESPONSE_HEADER_AGE;
extern const char* UC_HTTP_RESPONSE_HEADER_ETAG;
extern const char* UC_HTTP_RESPONSE_HEADER_LOCATION;
extern const char* UC_HTTP_RESPONSE_HEADER_PROXY_AUTHENTICATE;
extern const char* UC_HTTP_RESPONSE_HEADER_RETRY_AFTER;
extern const char* UC_HTTP_RESPONSE_HEADER_SERVER;
extern const char* UC_HTTP_RESPONSE_HEADER_VARY;
extern const char* UC_HTTP_RESPONSE_HEADER_WWW_AUTHENTICATE;
*/

#endif
