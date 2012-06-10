#ifndef __REQUEST_HANDLER_IF_H_
#define __REQUEST_HANDLER_IF_H_

#include <common/inc/SimpleVector.h>

class RequestHandlerIf_i;
class TlvAttrIf_i;
class TempMemBuffer_c;

class RequestItemIf_i
{
    public:
        RequestItemIf_i() :
            nSocketFd_m(0),
            pHandler_m(NULL)
        {
        }

        virtual ~RequestItemIf_i()
        {
        }

        bool init()
        {
            nSocketFd_m = 0;
            pHandler_m  = NULL;

            return true;
        }

        void unbind();

        virtual bool sendMessage(TempMemBuffer_c& tbBuffer);
        virtual void release() = 0;

        inline void setHandler(RequestHandlerIf_i* pHandler)    { pHandler_m = pHandler; }
        inline RequestHandlerIf_i* getHandler()                 { return pHandler_m; }

        // for hash table
        inline int* getFdAddress()          { return &nSocketFd_m; }
        inline int getSocketFd()            { return nSocketFd_m; }
        inline void setSocketFd(int nFd)    { nSocketFd_m = nFd; }
    protected:

    private:
        int nSocketFd_m;

        // for timeout tracking
        long long lStartTime_m;

        RequestHandlerIf_i* pHandler_m;
};

class RequestHandlerIf_i
{
    public:
        RequestHandlerIf_i() :
            pRequester_m(NULL)
        {
        }

        virtual ~RequestHandlerIf_i()
        {
        }

        virtual void processRequest(TempSimpleVector_c<TlvAttrIf_i*>* lstRequest) = 0;

        ////////////////////////////////////////////////////////////////////////////////
        // send message to requester
        // DON'T add version and tail attribute in message, they will be added before sending
        ////////////////////////////////////////////////////////////////////////////////
        virtual bool sendMessage(TempSimpleVector_c<TlvAttrIf_i*>* lstMessage);

        virtual void unbind()
        {
            // TODO: let client trigger the close?
            //if (NULL != pRequester_m)
            //{
            //    pRequester_m->release();
            //}
            if (NULL != pRequester_m)
            {
                pRequester_m->release();

                pRequester_m->unbind();

                // unbind requester and handler
                pRequester_m = NULL;
            }
        }

        inline void setRequester(RequestItemIf_i* pRequester)   { pRequester_m = pRequester; }
        inline RequestItemIf_i* getRequester()                  { return pRequester_m; }

    protected:
    private:
        RequestItemIf_i* pRequester_m;
};

#endif
