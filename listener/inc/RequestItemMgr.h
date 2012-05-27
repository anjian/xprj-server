#ifndef __REQUEST_ITEM_MGR_H_
#define __REQUEST_ITEM_MGR_H_

#include <common/inc/MyHash.h>
#include <common/inc/SimpleVector.h>

class RequestItem_c;
class TempMemBuffer_c;

class RequestItemMgr_c
{
    public:
        ~RequestItemMgr_c();
        static RequestItemMgr_c* getInstance();

        bool init()     { return true; }

        inline void setRequestHandler(request_handler_callback funcCB) { funcReqHandlerCB_m = funcCB; }

        bool handleRequest(int nFd, TempMemBuffer_c& dataReceived);

        void releaseReqItem(int nSocketFd);

    protected:
        RequestItemMgr_c();

        bool processRequest(RequestItem_c* pItem, const char* pRequest, int nLen);

        RequestItem_c* allocateReqItem();
        void releaseReqItem(RequestItem_c* pReqItem);

    private:
        request_handler_callback funcReqHandlerCB_m;

        HashInt_c<RequestItem_c*> htRequestItems_m;

        // request item object pool
        SimpleVector<RequestItem_c*> lstReqItemObj_m;
};

#endif
