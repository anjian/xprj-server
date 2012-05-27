
#include <interface/protocol/TlvIf.h>

#include <interface/listener/ListenerIf.h>
#include <interface/listener/RequestHandlerIf.h>

void RequestItemIf_i::unbind()
{
    if (NULL != pHandler_m)
    {
        pHandler_m->setRequester(NULL);
    }

    pHandler_m = NULL;
}

bool RequestItemIf_i::sendMessage(TempMemBuffer_c& tbBuffer)
{
    if (0 == nSocketFd_m)
    {
        return false;
    }

    IO_SOCKET_WRITE(nSocketFd_m, tbBuffer);
    return true;
}

bool RequestHandlerIf_i::sendMessage(TempSimpleVector_c<TlvAttrIf_i*>* lstMessage)
{
    if (NULL == getRequester())
    {
        return false;
    }

    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    // append protocol version
    TlvAttrIf_i* tlvVersion = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
    if (NULL == tlvVersion)
    {
        return false;
    }

    tlvVersion->setValue_int32(0x00010000);
    lstAttrs.append(tlvVersion);

    // append given message as content
    if (NULL != lstMessage)
    {
        for (int nIndex=0; nIndex<lstMessage->size(); nIndex++)
        {
            lstAttrs.append(lstMessage->get(nIndex));
        }
    }

    // append tail flag
    TlvAttrIf_i* tlvTail = createTlvAttribute(TLV_ATTR_TAIL_FLAG);
    if (NULL == tlvTail)
    {
        delete tlvVersion;
        return false;
    }
    tlvTail->setValue_int32(0);
    lstAttrs.append(tlvTail);

    // build message
    TempMemBuffer_c tbOutput;
    TlvEncoder_c tlvEncoder;
    tlvEncoder.generate(&lstAttrs, tbOutput);

    // send message to requester
    getRequester()->sendMessage(tbOutput);

    // release allocated internal items
    delete tlvVersion;
    delete tlvTail;

    return true;
}
