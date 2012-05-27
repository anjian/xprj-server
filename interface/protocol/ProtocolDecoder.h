#ifndef _UC_PROTOCOL_DECODER_H_
#define _UC_PROTOCOL_DECODER_H_

#define HTTP_DECODER_MIN_OUT_BUFFER_SIZE 256

//#include <platform/inc/ucweb_os_type.h>

//class UcUri_c;

enum DecodeRetStatus_e
{
    DECODE_STATUS_UNKNOWN,
    DECODE_STATUS_HEADER_CONTINUE,
    DECODE_STATUS_HEADER_COMPLETE,
    DECODE_STATUS_CONTINUE,
    DECODE_STATUS_COMPLETE,
    DECODE_STATUS_FAILURE
};

////////////////////////////////////////////////////////////////////////////////
// RFC2616 Chapter 10
////////////////////////////////////////////////////////////////////////////////
enum HttpResponseCode_e
{
    HTTP_RESP_CODE_UNKNOWN = -1,

    // Informational 1xx
    HTTP_RESP_CODE_100 = 100,   // continue
    HTTP_RESP_CODE_101,         // switcing protocol
    // Add new 1xx above this line
    HTTP_RESP_CODE_1xx_END,     // END of 1xx

    // Successful 2xx
    HTTP_RESP_CODE_200 = 200,   // OK
    HTTP_RESP_CODE_201,         // Created
    HTTP_RESP_CODE_202,         // Accepted
    HTTP_RESP_CODE_203,         // Non-Authoritative Information
    HTTP_RESP_CODE_204,         // No Content
    HTTP_RESP_CODE_205,         // Reset Content
    HTTP_RESP_CODE_206,         // Partial Content
    // Add new 2xx above this line
    HTTP_RESP_CODE_2xx_END,     // END of 2xx

    // Redirection 3xx
    HTTP_RESP_CODE_300 = 300,   // Multiple Choices
    HTTP_RESP_CODE_301,         // Moved Permanently
    HTTP_RESP_CODE_302,         // Found
    HTTP_RESP_CODE_303,         // See Other
    HTTP_RESP_CODE_304,         // Not Modified
    HTTP_RESP_CODE_305,         // Use Proxy
    HTTP_RESP_CODE_306,         // (Unused)
    HTTP_RESP_CODE_307,         // Temporary Redirect
    // Add new 3xx above this line
    HTTP_RESP_CODE_3xx_END,     // END of 3xx

    // Client Error 4xx
    HTTP_RESP_CODE_400 = 400,   // Bad Request
    HTTP_RESP_CODE_401,         // Unauthorized
    HTTP_RESP_CODE_402,         // Payment Required
    HTTP_RESP_CODE_403,         // Forbidden
    HTTP_RESP_CODE_404,         // Not Found
    HTTP_RESP_CODE_405,         // Method Not Allowed
    HTTP_RESP_CODE_406,         // Not Acceptable
    HTTP_RESP_CODE_407,         // Proxy Authentication Required
    HTTP_RESP_CODE_408,         // Request Timeout
    HTTP_RESP_CODE_409,         // Conflict
    HTTP_RESP_CODE_410,         // Gone
    HTTP_RESP_CODE_411,         // Length Required
    HTTP_RESP_CODE_412,         // Precondition Failed
    HTTP_RESP_CODE_413,         // Request Entity Too Large
    HTTP_RESP_CODE_414,         // Request-URI Too Long
    HTTP_RESP_CODE_415,         // Unsupported Media Type
    HTTP_RESP_CODE_416,         // Requested Range Not Satisfiable
    HTTP_RESP_CODE_417,         // Expectation Failed
    // Add new 4xx above this line
    HTTP_RESP_CODE_4xx_END,     // END of 4xx

    // Server Error 5xx
    HTTP_RESP_CODE_500 = 500,   // Internal Server Error
    HTTP_RESP_CODE_501,         // Not Implemented
    HTTP_RESP_CODE_502,         // Bad Gateway
    HTTP_RESP_CODE_503,         // Service Unavailable
    HTTP_RESP_CODE_504,         // Gateway Timeout
    HTTP_RESP_CODE_505,         // HTTP Version Not Supported
    // Add new 5xx above this line
    HTTP_RESP_CODE_5xx_END      // END of 5xx
};

////////////////////////////////////////////////////////////////////////////////
// Decoder interface
//
// decoder will handle memory buffer internally, it will response for allocating
// and freeing memory
//
// use DECORATE PATTERN to implement each concrete decoder
////////////////////////////////////////////////////////////////////////////////
class ProtocolDecoder_i
{
    public:
        ProtocolDecoder_i() :
            pDecoder_m(NULL)
        {
        }
        virtual ~ProtocolDecoder_i()
        {
            if (pDecoder_m)
            {
                delete pDecoder_m;
                pDecoder_m = NULL;
            }
        }

        virtual DecodeRetStatus_e decode(char* pIn, int nInLen, char** pOut, int& nOutLen) = 0;

        virtual void reset() {}
        virtual const char* getAttribute(int /*nIndex*/) const  { return NULL; }
#if 0

        // functions for http response 
        virtual UcHttpResponseCode_e getStatusCode()    { return HTTP_RESP_CODE_UNKNOWN; }
        virtual const char* getHeaderAttribute(int /*nIndex*/) const;
        virtual void end(char ** pOut,int & nOutLen);

        virtual void setUri(UcUri_c* /*pUri*/)          {}

        void setDecoder(UcProtocolDecoder_c* pDecoder);

        UcDecodeRetStatus_e subDecoderHandle(char * pIn,int nInLen,char ** pOut,int& nOutLen,UcDecodeRetStatus_e eRet);
#endif
    protected:
        ProtocolDecoder_i* pDecoder_m;
};

#endif
