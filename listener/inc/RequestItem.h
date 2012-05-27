#ifndef __REQUEST_ITEM_H_
#define __REQUEST_ITEM_H_

#include <interface/baseos/TempObject.h>
#include <interface/baseos/TempMemBuffer.h>

#include <interface/listener/RequestHandlerIf.h>

class TempMemBuffer_c;

class RequestItem_c : public RequestItemIf_i//: public TempObject_i
{
    public:
        RequestItem_c() :
            tbReqBuf_m(NULL)
        {
        }

        virtual ~RequestItem_c()
        {
            if (NULL != tbReqBuf_m)
            {
                delete tbReqBuf_m;
            }
        }

        bool init()
        {
            RequestItemIf_i::init();

            if (NULL != tbReqBuf_m)
            {
                delete tbReqBuf_m;
                tbReqBuf_m = NULL;
            }

            return true;
        }

        //////////////////////////////////////////////////////////////////////////////// 
        // RequestItemIf_i
        //////////////////////////////////////////////////////////////////////////////// 
        virtual void release();

        inline const char* getRequest()     { return (NULL != tbReqBuf_m) ? tbReqBuf_m->getBuffer() : NULL; }
        inline int getLength()              { return (NULL != tbReqBuf_m) ? tbReqBuf_m->getLength() : NULL; }

        inline void appendBuffer(const char* pBuf, int nLen)
        {
            if (NULL == tbReqBuf_m)
            {
                tbReqBuf_m = new TempMemBuffer_c();
                if (NULL == tbReqBuf_m)
                {
                    // error
                    return;
                }
            }

            tbReqBuf_m->append(pBuf, nLen);
        }

        inline void releaseBuffer()
        {
            if (NULL != tbReqBuf_m)
            {
                delete tbReqBuf_m;
                tbReqBuf_m = NULL;
            }
        }

    protected:
    private:
        TempMemBuffer_c* tbReqBuf_m;
};

#endif
