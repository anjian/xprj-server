#ifndef __CHAT_INFO_H_
#define __CHAT_INFO_H_

#include <interface/baseos/TempObject.h>
#include <interface/protocol/TlvValueDef.h>

class TempMemBuffer_c;

class ChatInfo_c : public TempObject_i
{
    public:
        ChatInfo_c();
        ChatInfo_c(ChatInfo_c* pChat);

        virtual ~ChatInfo_c();

        inline void setUserId(long long nId)    { nUserId_m = nId; }

        inline void setId(int nId)              { nId_m = nId; }
        inline void setGotTime(long long nTime) { nGotTime_m = nTime; }
        inline void setFrom(long long nId)      { nFrom_m = nId; }
        inline void setTo(long long nId)        { nTo_m = nId; }
        inline void setType(TlvAttrChatType_e eType)    { eType_m = eType; }
        inline void setAsRead()                 { bRead_m = true; }

        inline long long getUserId()            { return nUserId_m; }
        inline int getId()                      { return nId_m; }
        inline long long getGotTime()           { return nGotTime_m; }
        inline long long getFrom()              { return nFrom_m; }
        inline long long getTo()                { return nTo_m; }
        inline TlvAttrChatType_e getType()      { return eType_m; }
        inline const char* getChat()            { return tbChatInfo_m.getBuffer(); }
        inline int getChatLen()                 { return tbChatInfo_m.getLength(); }
        inline bool hasRead()                   { return bRead_m; }

        void appendChat(const char* pBuf, int nLen);

    protected:

    private:
        // keep user id who is handling this chat object
        long long nUserId_m;

        long long nGotTime_m;
        long long nFrom_m;
        long long nTo_m;

        int nId_m;
        bool bRead_m;

        TlvAttrChatType_e eType_m;
        TempMemBuffer_c tbChatInfo_m;
};

#endif
