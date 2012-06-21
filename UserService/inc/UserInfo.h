#ifndef __USER_INFO_H_
#define __USER_INFO_H_

#include <interface/listener/RequestHandlerIf.h>
#include <common/inc/SimpleVector.h>
//#include <common/inc/MyHash.h>

class ChatInfo_c;
class TlvAttrIf_i;

struct BuddyStatus_t
{
    long long nBuddyId_m;
    int nServiceGroupId_m;
};

class UserInfo_c : public RequestHandlerIf_i
{
    public:
        UserInfo_c();
        ~UserInfo_c();

        inline void setUserId(long long nId)            { nUserId_m = nId; }
        //inline void setServiceGroupId(int nId)          { nServiceGroupId_m = nId; }
        inline void setLevel(int nLevel)                { nLevel_m = nLevel; }

        void setScreenName(const char* sName);
        void setUserPixmap(const void* pBuf, int nLen);

        // for integer hash table key
        inline long long* getUserIdAddress()    { return &nUserId_m; }

        inline long long getUserId()            { return nUserId_m; }
        //inline int getServiceGroupId()          { return nServiceGroupId_m; }
        inline const char* getScreenName()      { return sScreenName_m; }
        inline int getUserLevel()               { return nLevel_m; }

        inline const void* getUserPixmap()      { return pPixBuffer_m; }
        inline int getUserPixmapLen()           { return nPixLen_m; }

        bool isOnline();

        void addBuddy(long long nBuddyId, int nSGId);
        void removeBuddy(long long nBuddyId);

        bool handleChatInfo(TlvAttrIf_i* pAttr);
        bool handleChatRespInfo(TlvAttrIf_i* pAttr);

        void receiveChat(TlvAttrIf_i* pAttr);

        ////////////////////////////////////////////////////////////////////////////////
        // RequestHandlerIf_i
        ////////////////////////////////////////////////////////////////////////////////
        virtual void processRequest(TempSimpleVector_c<TlvAttrIf_i*>* lstRequest);

        void debug();

    protected:

        bool handleLogout(TlvAttrIf_i* pAttr);

        void addChat(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat);
        void sendChatToBuddy(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat);
        bool sendChatByNet(int nSgId, TlvAttrIf_i* pTlvChat);

        void sendChatToClient(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat);

        void sendChatToUser(ChatInfo_c* pChat, TlvAttrIf_i* pTlvChat);
        bool sendChatRespMessage(int nChatId);

        int getBuddySGId(long long nBuddyId);

        // based on given message build ChatInfo content
        ChatInfo_c* buildChatInfo(TlvAttrIf_i* pTlvChat);

    private:
        long long nUserId_m;
        //int nServiceGroupId_m;
        char* sScreenName_m;
        int nLevel_m;

        long long nLastHeartBeat_m;

        void* pPixBuffer_m;
        int nPixLen_m;

        SimpleVector<BuddyStatus_t> lstBuddys_m;
        SimpleVector<ChatInfo_c*> lstChats_m;

        // chat request to user, waiting for the reponse from client
        SimpleVector<ChatInfo_c*> lstChatWaitingForRsp_m;
};

#if 0
class UserProfile_c
{
    public:
        UserProfile_c() {}
        ~UserProfile_c() {}

    protected:
    private:
};
#endif

#endif
