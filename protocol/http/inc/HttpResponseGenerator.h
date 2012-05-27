#ifndef __HTTP_RESPONSE_GENERATOR_H_
#define __HTTP_RESPONSE_GENERATOR_H_

//#include <platform/inc/ucweb_os_type.h>

#include <interface/protocol/DataOutputStream.h>

#define HTTP_SPACE                              " "
#define HTTP_END_OF_LINE                        "\r\n"

#define HTTP_CONNECTION_CLOSE                   "close"             // persistent connection disabled
#if 0
#define HTTP_ACCEPT_CHARSET_VALUE               "utf-8,*;q=0.7"     // We prefer utf-8
#define HTTP_ACCEPT_ENCODING_VALUE              "gzip"              // gzip,deflate
#define HTTP_ACCEPT_VALUE_NORMAL                "application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
#define HTTP_ACCEPT_VALUE_DOWNLOAD              "multipart/mixed,text/html,image/png,image/jpeg,image/gif,image/x-xbitmap,application/vnd.oma.dd+xml,*/*"
#endif

// TODO: to be configurable
#define HTTP_SERVER_DEF                         "XPRJ/1.0"
#define HTTP_CONTENT_TYPE_BIN                   "application/octet-stream"

#if 0
#define HTTP_ACCEPT_LANGUAGE_VALUE              "zh-cn,zh;q=0.5"
#define HTTP_CONTENT_TYPE_ATTR_BOUNDARY         "boundary"
#define HTTP_CONTENT_TYPE_NORMAL_VALUE          "text/plain"
#define HTTP_CONTENT_TYPE_URL_ENCODED           "application/x-www-form-urlencoded"
#define HTTP_CONTENT_TYPE_MULTI_PART_VALUE      "multipart/form-data"
#define HTTP_CONTENT_TYPE_MULTI_PART_BOUNDARY   "_multipart_boundary_1QAZ_5yuiop_3fMlka"
#endif


//class UcUri_c;
//class HttpRequest_c;

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

enum HttpResponseItemStatus_e
{
    HTTP_RSP_ITEM_STATUS_DONE,                  // the item has been handled successfully
    HTTP_RSP_ITEM_STATUS_COMPLETE,              // everything has been done
    HTTP_RSP_ITEM_STATUS_BUF_NOT_ENOUGH,        // buffer left available space is not enough
    HTTP_RSP_ITEM_STATUS_BUF_TOO_SHORT          // buffer is not enough even it's empty
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
enum HttpResponseStep_e
{
    HTTP_RESPONSE_STEP_START = 0,

    HTTP_RESPONSE_STEP_STATUS_LINE = HTTP_RESPONSE_STEP_START,
    HTTP_RESPONSE_STEP_CONNECTION,

    HTTP_RESPONSE_STEP_SERVER,
    HTTP_RESPONSE_STEP_CONTENT_TYPE,
    HTTP_RESPONSE_STEP_CONTENT_LENGTH,

    HTTP_RESPONSE_STEP_EMPTY_LINE,           // last step, give 'CRLF'

    // Please add available step above this line
    HTTP_RESPONSE_STEP_END
};

////////////////////////////////////////////////////////////////////////////////
// HttpResponseGenerator
//
// Generate http header based on received request. As a producer, it will not allocate
// buffer by itsef, instead, buffer will be organized by consumer (output user)
////////////////////////////////////////////////////////////////////////////////
class HttpResponseGenerator_c : public DataOutputStream_i
{
    //funtion table
    public:
        // event handler function structure
        struct HttpResponseHandlerTable_t
        {
            //UcHttpRequestStep_e eStep_m;            // Indeed, we don't need this...
            int (HttpResponseGenerator_c::*getEstimateSize)();
            void (HttpResponseGenerator_c::*stepHandler)(char*);
        };
        static HttpResponseHandlerTable_t pHttpReqHandler_m[];

    public:
        enum HttpResponseFlagStatus_e
        {
            HTTP_RESPONSE_FLAG_NOT_SET   = 0,
            HTTP_RESPONSE_FLAG_SET       = 1
        };

    public:
        HttpResponseGenerator_c();
        virtual ~HttpResponseGenerator_c();
        
        bool init() { return true; }

        ////////////////////////////////////////////////////////////////////////////////
        // Output stream interface
        ////////////////////////////////////////////////////////////////////////////////
        OutputStreamRetCode_e generate(char* sBuf, const int nBufLen, int& nContentLen);
        int getContentLength()     { return 0; }
        void reset();

        ////////////////////////////////////////////////////////////////////////////////
        // http response flag
        ////////////////////////////////////////////////////////////////////////////////
        inline void setHttpContentLength(int nContentLength)      { nContentLength_m = nContentLength; }
#if 0
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
#endif

        //inline void setReferer(UcHttpRequestRefererType_e eReferer)
        //{
        //    nRefererType_m = ((HTTP_REQ_REFERER_START <= eReferer) && (eReferer < HTTP_REQ_REFERER_END))
        //        ? eReferer : HTTP_REQ_REFERER_NONE;
        //}
#if 0
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
#endif

    protected:
        // get estimate size, it should be a little bigger than actrual one
        int getStatusLineEstimateSize();
        int getServerEstimateSize();
        int getConnectionEstimateSize();
        int getContentTypeEstimateSize();
        int getContentLengthEstimateSize();

        int getEmptyLineSize();

        // http attribute handler
        void statusLineHandler(char* sBuf);
        void serverHandler(char* sBuf);
        void connectionHandler(char* sBuf);
        void contentTypeHandler(char* sBuf);
        void contentLengthHandler(char* sBuf);

        void emptyLineHandler(char* sBuf);

    protected:
        HttpResponseItemStatus_e stepHandlerFrame(char* sBuf, const int nBufLen, int& nContentLen);
        // go to next step
        inline void stepCompleted()     { eCurStep_m = (HttpResponseStep_e)(eCurStep_m + 1); }

        void appendBuffer(char* sBuf, char* sPart, const int nBufLen, int& nContentLen);

        //void assembleHost(char* sBuf, UcUri_c* pUri, bool bWithScheme);

    private:
        HttpResponseStep_e eCurStep_m;

        int nContentLength_m;

#if 0

        // range info, available when bRangeSet_m is set
        UC_INT32 nFirstBytePos_m;
        UC_INT32 nLastBytePos_m;

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
#endif
};

#endif
