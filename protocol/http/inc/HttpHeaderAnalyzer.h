#ifndef __HTTP_HEADER_ANALYZER_H_
#define __HTTP_HEADER_ANALYZER_H_

//#include <platform/inc/ucweb_os_type.h>

#include <common/inc/SimpleVector.h>

#include <interface/baseos/TempMemBuffer.h>
#include <interface/protocol/ProtocolDecoder.h>

//#include <protocol/inc/UcWebDataInputStream.h>
#include "protocol/http/inc/HttpHeader.h"


//class ProtocolDecoder_c;
//class Uri_c;

struct HttpAttrInfo_t
{
    int nHeaderIndex_m;             // From HttpHeader.h
    char* sValue_m;                 // new allocated, to be freed
};

class HttpHeaderAnalyzer_c : public ProtocolDecoder_i
{
    public:
        enum HttpMessageType_e
        {
            HTTP_REQUEST,
            HTTP_RESPONSE,
        };

    public:
        HttpHeaderAnalyzer_c();
        virtual ~HttpHeaderAnalyzer_c();

        virtual DecodeRetStatus_e decode(char* sBuf, int nLen, char** sOutBuf, int& nOutLen);
        virtual const char* getAttribute(int nIndex) const;

        virtual void reset();

        void disableHeaderParse();

        //inline int getReceivedBodyLen() { return nBodyLength_m; }

        inline HttpResponseCode_e getStatusCode()      { return nStatusCode_m; }
        inline bool isRedirect()
        {
            return (HTTP_RESP_CODE_300 <= nStatusCode_m) && (nStatusCode_m < HTTP_RESP_CODE_3xx_END);
        }

        int getContentLength();

        // disable decoder chain 
        //void setDisableDecoderChain(bool bDisableDecoderChain) { bDisableDecoderChain_m = bDisableDecoderChain ; } 
       

        //inline void setLpFileFlag()  { bLpFileFlag_m = true;}
            
#if 0
        void setNeedDocTypeScanner(UC_BOOL bBool){ bNeedDocTypeScanner_m = bBool ? 1 : 0; }
        UC_BOOL isNeedDocTypeScanner(){ return bNeedDocTypeScanner_m == 1 ? UC_TRUE : UC_FALSE; }
            
        void setNeedCharsetDecoder(UC_BOOL bBool){ bNeedCharsetDecoder_m = bBool ? 1 : 0; }
        UC_BOOL isNeedCharsetDecoder(){ return bNeedCharsetDecoder_m == 1 ? UC_TRUE : UC_FALSE; }
#endif

        // for debug
        void printHeader();
    protected:
        void parseHeader(char* sBuf, int nLength);
        //void handleCookieSet();
        void releaseDecoder();
        bool setHttpDecoder();

        bool retrieveReqMethod(char* sBuf, int nLength);
        bool retrieveStatusCode(char* sBuf, int nLength);
        bool retrieveAttribute(char* sBuf, int nLength);
        void getHeaderValue(char* sBuf, int nLength, char** sValue, int& nValueLen);
        void appendHeaderItem(char* sValue, int nLength, int keyIndex);
        bool trimBeginToDigit(char*& sValue, int& nLen);

        bool isEmptyLine(char* sBuf, int nLength);

        bool jumpToNextLine(char** sBuf, int& nLength);
        bool jumpToCharacter(char** sBuf, int& nLength, char ch);
        bool jumpToNonSpace(char** sBuf, int& nLength);

    private:
        void clearHeaderInfo();

    private:
        HttpMessageType_e eMsgType_m;
        HttpResponseCode_e nStatusCode_m;

        bool bIsHeaderReady_m;

        // temporary buffer, it's used to keep the last http header attribute 
        // which is not received completely
        //
        // it's to be allocated in transient memory pool
        //char* sHeaderBuffer_m;
        //int nBufferLen_m;
        //int nContentLen_m;
        TempMemBuffer_c mbHeaderBuffer_m;

        int nBodyLength_m;

#if 0
        UC_BOOL bDisableDecoderChain_m;

        UC_BOOL bNeedDocTypeScanner_m : 4;
        UC_BOOL bNeedCharsetDecoder_m : 4;
#endif

        // To reduce memory allocation times, use item's list rather than the pointer here
        SimpleVector<HttpAttrInfo_t> lstHeaderInfos_m;
};

#endif
