#ifndef __REQUEST_HANDLER_H_
#define __REQUEST_HANDLER_H_

#include <common/inc/SimpleVector.h>

class TlvAttrIf_i;
class RequestItemIf_i;

bool clientRequestHandler(RequestItemIf_i* pReqItem, TempSimpleVector_c<TlvAttrIf_i*>* pRequest);

#if 0
class RequestHandler_c
{
    public:
        ~RequestHandler_c();

        static RequestHandler_c* getInstance();

        bool process(int nFd, TempMemBuffer_c& dataReceived, TempMemBuffer_c& dataToSend);

    protected:
        RequestHandler_c();

    private:
};
#endif

#endif
