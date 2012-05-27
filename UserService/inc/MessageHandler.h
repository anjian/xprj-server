#ifndef __MESSAGE_HANDLER_H_
#define __MESSAGE_HANDLER_H_

#include <common/inc/SimpleVector.h>

class TlvAttrIf_i;

bool serverResponseHandler(TempSimpleVector_c<TlvAttrIf_i*>* pResponse);

#if 0
class TempMemBuffer_c;

class MessageHandler_c
{
    public:
        ~MessageHandler_c();

        static MessageHandler_c* getInstance();

        bool process(int nFd, TempMemBuffer_c& dataReceived, TempMemBuffer_c& dataToSend);

    protected:
        MessageHandler_c();

    private:
};
#endif

#endif
