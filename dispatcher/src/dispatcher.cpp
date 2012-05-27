#include <common/inc/MyDebug.h>
#include <common/inc/MyHash.h>

#include <interface/baseos/TempMemBuffer.h>
#include <interface/listener/ListenerIf.h>

#include <dispatcher/inc/RequestHandler.h>

#include <interface/protocol/TlvIf.h>
#include <interface/protocol/tlv.h>

#define DISPATCHER_SERVER_PORT      "8081"

bool tlvTest()
{
    TempSimpleVector_c<TlvAttrIf_i*> lstAttrs(8);

    ////////////////////////////////////////////////////////////////////////////////
    // encode
    ////////////////////////////////////////////////////////////////////////////////
    // append protocol version
    TlvAttrIf_i* tlvTest = createTlvAttribute(TLV_ATTR_CLIENT_VERSION);
    tlvTest->setValue_int32(0x00010000);

    lstAttrs.append(tlvTest);

    // set attribute test
    {
        TlvAttrIf_i* tlvUserInfo = createTlvAttribute(TLV_ATTR_USER_INFO);

        // user id
        TlvAttrIf_i* tlvUserId = createTlvAttribute(TLV_ATTR_USER_ID);
        tlvUserId->setValue_int64(0x0102030405060708L);
        tlvUserInfo->appendAttr(tlvUserId);
        
        // password
        TlvAttrIf_i* tlvUserPwd = createTlvAttribute(TLV_ATTR_USER_PASSWORD);
        tlvUserPwd->setValue_buffer("test", 4);
        tlvUserInfo->appendAttr(tlvUserPwd);

        lstAttrs.append(tlvUserInfo);
    }

    // append tail flag
    TlvAttrIf_i* tlvTail = createTlvAttribute(TLV_ATTR_TAIL_FLAG);
    tlvTail->setValue_int32(0);

    lstAttrs.append(tlvTail);

    TempMemBuffer_c tbOutput;
    TlvEncoder_c tlvEncoder;
    tlvEncoder.generate(&lstAttrs, tbOutput);

    for (int nIndex=lstAttrs.size()-1; nIndex>=0; nIndex--)
    {
        delete lstAttrs.takeLast();
    }


    ////////////////////////////////////////////////////////////////////////////////
    // encode
    ////////////////////////////////////////////////////////////////////////////////
    //
    TempSimpleVector_c<TlvAttrIf_i*> lstDecodedAttrs(8);

    TlvParser_c tlvParser(&lstDecodedAttrs);
    const char* pBuffer = tbOutput.getBuffer();
    int nLeftLen        = tbOutput.getLength();

    int nStep           = 1;

    for (; nLeftLen > 0; nLeftLen -= nStep)
    {
        int nDecode = (nLeftLen > nStep) ? nStep : nLeftLen;
        if (TLV_PARSE_DONE == tlvParser.parse(pBuffer, nDecode))
        {
            MSG("decoded attributes: %d\n", lstDecodedAttrs.size());
        }

        pBuffer += nStep;
    }

    return true;
}

int main()
{
    MSG("Service Starting: User Dispatcher.\n");

    //tlvTest();

    SET_SERVER_REQUEST_HANDLER(requestHandler);
    IO_SOCKET_START_LISTENER(DISPATCHER_SERVER_PORT);

    return 0;
}
