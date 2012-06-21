
#include <common/inc/MyTime.h>

#include <interface/protocol/TlvValueDef.h>
#include <interface/baseos/TempMemBuffer.h>

#include <UserService/inc/ChatInfo.h>

ChatInfo_c::ChatInfo_c() :
    nFrom_m(0),
    nTo_m(0),
    nId_m(0),
    bRead_m(false),
    eType_m(TLV_CHAT_TYPE_TEXT)
{
    nGotTime_m = Time_c::getTimestamp();
}

ChatInfo_c::ChatInfo_c(ChatInfo_c* pChat)
{
    if (NULL != pChat)
    {
        nGotTime_m      = pChat->nGotTime_m;
        nFrom_m         = pChat->nFrom_m;
        nTo_m           = pChat->nTo_m;
        nId_m           = pChat->nId_m;
        bRead_m         = pChat->bRead_m;

        eType_m         = pChat->eType_m;
        tbChatInfo_m.append(pChat->tbChatInfo_m.getBuffer(), pChat->tbChatInfo_m.getLength());
    }
    else
    {
        nFrom_m = 0;
        nTo_m   = 0;
        nId_m   = 0;
        eType_m = TLV_CHAT_TYPE_TEXT;
        nGotTime_m = Time_c::getTimestamp();
    }
}

ChatInfo_c::~ChatInfo_c()
{
}

void ChatInfo_c::appendChat(const char* pBuf, int nLen)
{
    if ((NULL == pBuf) || (0 >= nLen))
    {
        return;
    }

    tbChatInfo_m.append(pBuf, nLen);
}

