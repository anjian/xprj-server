
#include <common/inc/MyDebug.h>
#include <common/inc/MyStdLibrary.h>

#include <interface/baseos/TempMemBuffer.h>

#include <interface/protocol/TlvIf.h>

//#include <UserService/inc/ServiceGroupInfo.h>
#include <UserService/inc/MessageHandler.h>

#if 0
MessageHandler_c::MessageHandler_c()
{
}

MessageHandler_c::~MessageHandler_c()
{
}

IMPLE_OBJECT_GET_INSTANCE(MessageHandler_c)

bool MessageHandler_c::process(int nFd, TempMemBuffer_c& dataReceived, TempMemBuffer_c& dataToSend)
{
    IoTransIf_i* pIo = IO_SOCKET_GET_TRANS(nFd);

    if (NULL == pIo)
    {
        // server process
        dataReceived.cutAsString();
        MSG("SERVER RECEIVED: %s\n", dataReceived.getBuffer());

        const char* sEcho = "echo back";

        dataToSend.append(sEcho, xstrlen(sEcho));
    }
    else
    {
        // As client, got server response 
        pIo->received(dataReceived);
    }

    return true;
}
#endif

bool serverResponseHandler(TempSimpleVector_c<TlvAttrIf_i*>* pResponse)
{
    // handle content
    for (int nIndex=1; nIndex<pResponse->size(); nIndex++)
    {
        TlvAttrIf_i* pAttr = pResponse->get(nIndex);
        switch (pAttr->getType())
        {
            case TLV_ATTR_SERVICE_GROUP_UPDATE:
                {
                    //ServiceGroupMgr_c::getInstance()->handleServiceGroupUpdate(pAttr);
                }
                break;
            case TLV_ATTR_CLIENT_VERSION:
            case TLV_ATTR_TAIL_FLAG:
                {
                }
                break;
            default:;
        }
    }

    return true;
}
