#ifndef __HTTP_REQUEST_H_
#define __HTTP_REQUEST_H_

//#include <platform/inc/ucweb_os_type.h>

//#include <interface/protocol/UcWebDataOutputStream.h>

#define HTTP_SPACE                              " "
#define HTTP_END_OF_LINE                        "\r\n"

#define HTTP_CONNECTION_CLOSE                   "close"             // persistent connection disabled
#define HTTP_ACCEPT_CHARSET_VALUE               "gb2312,utf-8;q=0.7,*;q=0.7"     // We prefer utf-8
#define HTTP_ACCEPT_ENCODING_VALUE              "gzip"              // gzip,deflate
#define HTTP_ACCEPT_VALUE_NORMAL                "application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
#define HTTP_ACCEPT_VALUE_DOWNLOAD              "multipart/mixed,text/html,image/png,image/jpeg,image/gif,image/x-xbitmap,application/vnd.oma.dd+xml,*/*"

#define HTTP_ACCEPT_LANGUAGE_VALUE              "zh-cn,zh;q=0.5"

#define HTTP_CONTENT_TYPE_ATTR_BOUNDARY         "boundary"
#define HTTP_CONTENT_TYPE_NORMAL_VALUE          "text/plain"
#define HTTP_CONTENT_TYPE_URL_ENCODED           "application/x-www-form-urlencoded"
#define HTTP_CONTENT_TYPE_MULTI_PART_VALUE      "multipart/form-data"
#define HTTP_CONTENT_TYPE_MULTI_PART_BOUNDARY   "_multipart_boundary_1QAZ_5yuiop_3fMlka"


//class UcUri_c;
class HttpRequest_c;

////////////////////////////////////////////////////////////////////////////////
// Http Request Method
////////////////////////////////////////////////////////////////////////////////
enum HttpRequestMethod_e
{
    HTTP_REQ_METHOD_START   = 0,
    HTTP_REQ_METHOD_GET     = HTTP_REQ_METHOD_START,
    HTTP_REQ_METHOD_HEADER,
    HTTP_REQ_METHOD_POST,

    HTTP_REQ_METHOD_END
};

////////////////////////////////////////////////////////////////////////////////
// Http Request Method: End
////////////////////////////////////////////////////////////////////////////////
//
//enum UcOutputStreamRetCode_e
//{
//    OUTPUT_GEN_COMPLETE,
//    OUTPUT_GEN_CONTINUE,
//    OUTPUT_GEN_BUF_NOT_ENOUGH
//};

enum HttpRequestItemStatus_e
{
    HTTP_REQ_ITEM_STATUS_DONE,                  // the item has been handled successfully
    HTTP_REQ_ITEM_STATUS_COMPLETE,              // everything has been done
    HTTP_REQ_ITEM_STATUS_BUF_NOT_ENOUGH,        // buffer left available space is not enough
    HTTP_REQ_ITEM_STATUS_BUF_TOO_SHORT          // buffer is not enough even it's empty
};

////////////////////////////////////////////////////////////////////////////////
// Indeed it's the state of Http request generator, and used to track http 
// request header generation. Named as this only for fitting this circumstance:
// 
// In order to simplify the transition handling, it'll go step by step just as
// the defintion
// 
// Step to introduce a new header item:
//  1) Add new step type in UcHttpRequestStep_e (before line HTTP_REQUEST_STEP_END)
//  2) Add cooresponding function declaration in UcHttpRequest_c
//      2.1 get size function: int getXXXEstimateSize()
//      2.2 handler function:  void XXXHandler(char*)
//  3) Add these info into handler table pHttpReqHandler_m in cpp file
//      !!! NOTE: ensure the sequence is exactly the same as UcHttpRequestStep_e !!!
//  4) Implement getXXXEstimateSize & XXXHandler 
// That's All!
////////////////////////////////////////////////////////////////////////////////
enum HttpRequestStep_e
{
    HTTP_REQUEST_STEP_START = 0,

    HTTP_REQUEST_STEP_REQUEST_LINE = HTTP_REQUEST_STEP_START,
    HTTP_REQUEST_STEP_HOST,
    HTTP_REQUEST_STEP_CONNECTION,
    HTTP_REQUEST_STEP_ACCEPT_CHARSET,
    HTTP_REQUEST_STEP_ACCEPT_ENCODING,
    HTTP_REQUEST_STEP_ACCEPT_LANGUAGE,
    HTTP_REQUEST_STEP_USER_AGENT,
    HTTP_REQUEST_STEP_RANGE,
    HTTP_REQUEST_STEP_CONTENT_TYPE,
    HTTP_REQUEST_STEP_REFERER,
    HTTP_REQUEST_STEP_ACCEPT,
    HTTP_REQUEST_STEP_CONTENT_LENGTH,
    HTTP_REQUEST_STEP_COOKIE,
    HTTP_REQUEST_STEP_EMPTY_LINE,           // last step, give 'CRLF'

    // Please add available step above this line
    HTTP_REQUEST_STEP_END
};

enum UcHttpRequestAcceptType_e
{
    HTTP_REQ_ACCEPT_START,
    HTTP_REQ_ACCEPT_NORMAL,
    HTTP_REQ_ACCEPT_DOWNLOAD,

    HTTP_REQ_ACCEPT_END
};

enum UcHttpReqContentType_e
{
    HTTP_REQ_CONTENT_TYPE_NORMAL,
    HTTP_REQ_CONTENT_TYPE_URL_ENCODED,
    HTTP_REQ_CONTENT_TYPE_MULTI_PART
};

////////////////////////////////////////////////////////////////////////////////
// HttpRequest_c
//
// Generate http header based on given uri. As a producer, it will not allocate
// buffer by itsef, instead, buffer will be organized by consumer (output user)
////////////////////////////////////////////////////////////////////////////////
class HttpRequest_c //: public UcWebDataOutputStream_c
{
    //funtion table
    public:
        // event handler function structure
        struct HttpRequestHandlerTable_t
        {
            //UcHttpRequestStep_e eStep_m;            // Indeed, we don't need this...
            int (HttpRequest_c::*getEstimateSize)();
            void (HttpRequest_c::*stepHandler)(char*);
        };
        static UcHttpRequestHandlerTable_t pHttpReqHandler_m[];

    public:
        enum UcHttpRequestFlagStatus_e
        {
            HTTP_REQ_FLAG_NOT_SET   = 0,
            HTTP_REQ_FLAG_SET       = 1
        };

    public:
        HttpRequest_c(UcUri_c* uri);
        HttpRequest_c();
        ~HttpRequest_c();
        
        bool init() { return UC_TRUE; }

        void setUri(UcUri_c* uri);
        inline UcUri_c* getUri()    { return pUri_m; }

        ////////////////////////////////////////////////////////////////////////////////
        // Output stream interface
        ////////////////////////////////////////////////////////////////////////////////
        UcOutputStreamRetCode_e generate(UC_CHAR* sBuf, const UC_INT32 nBufLen, UC_INT32& nContentLen);
        UC_INT32 getContentLength()     { return 0; }
        void reset();

        ////////////////////////////////////////////////////////////////////////////////
        // http request flag
        ////////////////////////////////////////////////////////////////////////////////
        inline void setHttpContentLength(UC_INT32 nContentLength)      { nContentLength_m = nContentLength; }
        inline void setMethod(UcHttpRequestMethod_e eMethod)
        {
            nReqMethod_m = ((HTTP_REQ_METHOD_START <= eMethod) && (eMethod < HTTP_REQ_METHOD_END)) ?
                eMethod : HTTP_REQ_METHOD_GET;
        }

        inline void setAccept(UcHttpRequestAcceptType_e eAccept)
        {
            nAcceptType_m = ((HTTP_REQ_ACCEPT_START <= eAccept) && (eAccept < HTTP_REQ_ACCEPT_END)) ?
                eAccept : HTTP_REQ_ACCEPT_NORMAL;
        }

        //inline void setReferer(UcHttpRequestRefererType_e eReferer)
        //{
        //    nRefererType_m = ((HTTP_REQ_REFERER_START <= eReferer) && (eReferer < HTTP_REQ_REFERER_END))
        //        ? eReferer : HTTP_REQ_REFERER_NONE;
        //}
        inline void setRefererUri(UcUri_c* uri) { pRefererUri_m = uri; }
        inline UcUri_c* getRefererUri()         { return pRefererUri_m; }

        inline void enableProxy()       { bUseProxy_m = HTTP_REQ_FLAG_SET; }
        inline void disableProxy()      { bUseProxy_m = HTTP_REQ_FLAG_NOT_SET; }

        inline void enableCookie()      { bCookieEnabled_m = UC_TRUE;}        
        inline void disableCookie()     { bCookieEnabled_m = UC_FALSE;}

        inline void setNormalContent()      { nContentType_m = HTTP_REQ_CONTENT_TYPE_URL_ENCODED; }
        inline void setMultiPartContent()   { nContentType_m = HTTP_REQ_CONTENT_TYPE_MULTI_PART; }
        inline void setUrlEncodedContent()  { nContentType_m = HTTP_REQ_CONTENT_TYPE_URL_ENCODED; }
        void setCookies(const UC_CHAR* sCookieData);
        
        //inline void setContentLenFlag()  { bContentLengthNeeded_m = HTTP_REQ_FLAG_SET;}
        //inline void resetContentLenFlag(){ bContentLengthNeeded_m = HTTP_REQ_FLAG_NOT_SET;}

        void setRange(UC_INT32 nFPos, UC_INT32 nLPos);

    protected:
        // get estimate size, it should be a little bigger than actrual one
        int getRequestLineEstimateSize();
        int getHostEstimateSize();
        int getConnectionSize();
        int getAcceptCharsetSize();
        int getAcceptEncodingSize();
        int getAcceptLanguageSize();
        int getUserAgentEstimateSize();
        int getRangeEstimateSize();
        int getContentTypeEstimateSize();
        int getRefererEstimateSize();
        int getContentLengthSize();
        int getAcceptSize();
        int getCookieSize();
        int getEmptyLineSize();

        void requestLineHandler(char* sBuf);
        void hostHandler(char* sBuf);
        void connectionHandler(char* sBuf);
        void acceptCharsetHandler(char* sBuf);
        void acceptEncodingHandler(char* sBuf);
        void acceptLanguageHandler(char* sBuf);
        void userAgentHandler(char* sBuf);
        void rangeHandler(char* sBuf);
        void contentTypeHandler(char* sBuf);
        void refererHandler(char* sBuf);
        void contentLengthHandler(char* sBuf);
        void acceptHandler(char* sBuf);
        void cookieHandler(char* sBuf);
        void emptyLineHandler(char* sBuf);

    protected:
        UcHttpRequestItemStatus_e stepHandlerFrame(char* sBuf, const int nBufLen, int& nContentLen);
        // go to next step
        inline void stepCompleted()     { eCurStep_m = (UcHttpRequestStep_e)(eCurStep_m + 1); }

        void appendBuffer(char* sBuf, char* sPart, const int nBufLen, int& nContentLen);

        void assembleHost(char* sBuf, UcUri_c* pUri, bool bWithScheme);

    private:
        UcHttpRequestStep_e eCurStep_m;

        // it's given by consumer, and need not release 
        UcUri_c* pUri_m;
        UcUri_c* pRefererUri_m;

        // range info, available when bRangeSet_m is set
        UC_INT32 nFirstBytePos_m;
        UC_INT32 nLastBytePos_m;

        UC_INT32 nContentLength_m;

        // flags
        UC_INT32 nReqMethod_m:4;        // request method
        UC_INT32 bUseProxy_m:2;         // Is this request to go through proxy 
        UC_INT32 bRangeSet_m:2;         // Is the range set
        UC_INT32 nContentType_m:4;      // content type
        //UC_INT32 nRefererType_m:4;      // NONE | absoluteUri | absolutePath, default is NONE
        UC_INT32 nAcceptType_m:4;         // NORMAL | DOWNLOAD, default is NORMAL
        //unsigned int bContentLengthNeeded_m:1;
        UC_INT32 bCookieEnabled_m:2; //for css/img, no cookie.
        UC_INT32 nDummy_m:14;

        //to buffering the cookie data       
        //because it is a little time wasted to access the cookie mgr database       
        //the cookie data is retrieved when getting the cookie length        
        UC_CHAR* sCookieData_m;        
};

#endif
