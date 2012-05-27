#include "protocol/xmllite/test/inc/UcWebTagInfoTest.h"
#include "com/inc/UcDebug.h"
#include "com/inc/UcStdLibrary.h"
#include "com/inc/UcLib.h"
#include "interface/protocol/UcWebTagInfo.h"
#include <stdio.h>

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UcWebTagInfoTest_c, "testWebTagInfo");

void runWebTagInfoTest()
{
     runTestCase("testWebTagInfo","WebTagInfo.log");
}

void UcWebTagInfoTest_c::setUp()
{
}

void UcWebTagInfoTest_c::tearDown()
{
}

void UcWebTagInfoTest_c::testTagCompleteness()
{
    const UcWebTagInfo_t* pInfo = debug_getTagInfo(WEB_TAG_TYPE_W3C_START);
    for(UC_INT32 nIndex = WEB_TAG_TYPE_W3C_START; nIndex < WEB_TAG_TYPE_W3C_END; nIndex++)
    {
        pInfo = debug_getTagInfo(nIndex);
        if(pInfo->eTagType_m != nIndex)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(pInfo->eTagType_m == nIndex);
    }
}

void UcWebTagInfoTest_c::testAtrributeCompleteness()
{
    const UcWebAttributeInfo_t* pInfo = debug_getAttriInfo(WEB_ATTRIBUTE_TYPE_ABBR);
    for(UC_INT32 nIndex = WEB_ATTRIBUTE_TYPE_ABBR; nIndex < WEB_ATTRIBUTE_TYPE_END; nIndex++)
    {
        pInfo = debug_getAttriInfo(nIndex);
        if(pInfo->eAttrType_m != nIndex)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(pInfo->eAttrType_m == nIndex);
    }
}

void UcWebTagInfoTest_c::testTagArrayIsAscendingAlphabeticOrder()
{
    const UcWebTagInfo_t* pInfo = NULL;
    const UcWebTagInfo_t* pPreInfo = NULL;
    for(UC_INT32 nIndex = WEB_TAG_TYPE_W3C_START + 1; nIndex < WEB_TAG_TYPE_W3C_END; nIndex++)
    {
        pPreInfo = debug_getTagInfo(nIndex - 1);
        pInfo = debug_getTagInfo(nIndex);
        if(compareIc(pPreInfo->s_name, uc_strlen(pPreInfo->s_name), pInfo->s_name) >= 0)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(compareIc(pPreInfo->s_name, uc_strlen(pPreInfo->s_name), pInfo->s_name) < 0);
    }
}

void UcWebTagInfoTest_c::testAttribArrayIsAscendingAlphabeticOrder()
{
    const UcWebAttributeInfo_t* pInfo = NULL;
    const UcWebAttributeInfo_t* pPreInfo = NULL;
    for(UC_INT32 nIndex = WEB_ATTRIBUTE_TYPE_ABBR + 1; nIndex < WEB_ATTRIBUTE_TYPE_END; nIndex++)
    {
        pPreInfo = debug_getAttriInfo(nIndex - 1);
        pInfo = debug_getAttriInfo(nIndex);
        if(compareIc(pPreInfo->s_name, uc_strlen(pPreInfo->s_name), pInfo->s_name) >= 0)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(compareIc(pPreInfo->s_name, uc_strlen(pPreInfo->s_name), pInfo->s_name) < 0);
    }
}


//interface test
void UcWebTagInfoTest_c::testGetTagInfo()
{
    //const Wap2Keyword* getTagInfo(const UC_CHAR* sTagName, UC_INT32 nLen);
    const UcWebTagInfo_t* pInfo = debug_getTagInfo(WEB_TAG_TYPE_W3C_START);
    for(UC_INT32 nIndex = WEB_TAG_TYPE_W3C_START; nIndex < WEB_TAG_TYPE_W3C_END; nIndex++)
    {
        pInfo = debug_getTagInfo(nIndex);
        const Wap2Keyword* pKeyWord = getTagInfo(pInfo->s_name, uc_strlen(pInfo->s_name));
        if(pKeyWord->token != nIndex)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(pKeyWord->token == nIndex);
    }

    CPPUNIT_ASSERT(NULL == getTagInfo(NULL, 0));
    CPPUNIT_ASSERT(NULL == getTagInfo("adfkdfeijsldkfjlksd", uc_strlen("adfkdfeijsldkfjlksd")));
}

void UcWebTagInfoTest_c::testGetAttriInfo()
{
    //const Wap2Keyword* getAttriInfo(const UC_CHAR* sAttriName, UC_INT32 nLen);
    const UcWebAttributeInfo_t* pInfo = debug_getAttriInfo(WEB_ATTRIBUTE_TYPE_ABBR);
    for(UC_INT32 nIndex = WEB_ATTRIBUTE_TYPE_ABBR; nIndex < WEB_ATTRIBUTE_TYPE_END; nIndex++)
    {
        pInfo = debug_getAttriInfo(nIndex);
        const Wap2Keyword* pKeyWord = getAttriInfo(pInfo->s_name, uc_strlen(pInfo->s_name));
        if(pKeyWord->token != nIndex)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(pKeyWord->token == nIndex);
    }
    CPPUNIT_ASSERT(NULL == getAttriInfo(NULL, 0));
    CPPUNIT_ASSERT(NULL == getAttriInfo("adfkdfeijsldkfjlksd", uc_strlen("adfkdfeijsldkfjlksd")));
}

void UcWebTagInfoTest_c::testGetTagName()
{
    //const UC_CHAR* getTagName(UC_INT32 nToken);
    const UcWebTagInfo_t* pInfo = debug_getTagInfo(WEB_TAG_TYPE_W3C_START);
    for(UC_INT32 nIndex = WEB_TAG_TYPE_W3C_START; nIndex < WEB_TAG_TYPE_W3C_END; nIndex++)
    {
        pInfo = debug_getTagInfo(nIndex);
        const UC_CHAR* pTagName = getTagName(pInfo->eTagType_m);
        if(0 != uc_strcmp(pTagName, pInfo->s_name))
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(0 == uc_strcmp(pTagName, pInfo->s_name));
    }

    CPPUNIT_ASSERT(NULL == getTagName(2091));
}

void UcWebTagInfoTest_c::testGetAttriName()
{
    //const UC_CHAR* getAttriName(UC_INT32 nToken);
    const UcWebAttributeInfo_t* pInfo = debug_getAttriInfo(WEB_ATTRIBUTE_TYPE_ABBR);
    for(UC_INT32 nIndex = WEB_ATTRIBUTE_TYPE_ABBR; nIndex < WEB_ATTRIBUTE_TYPE_END; nIndex++)
    {
        pInfo = debug_getAttriInfo(nIndex);
        const UC_CHAR* pAttriName = getAttriName(pInfo->eAttrType_m);
        if(0 != uc_strcmp(pAttriName, pInfo->s_name))
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(0 == uc_strcmp(pAttriName, pInfo->s_name));
    }

    CPPUNIT_ASSERT(NULL == getAttriName(2091));
}

void UcWebTagInfoTest_c::testGetTagIndex()
{
    //const Wap2Keyword* getTagInfo(const UC_CHAR* sTagName, UC_INT32 nLen);
    const UcWebTagInfo_t* pInfo = debug_getTagInfo(WEB_TAG_TYPE_W3C_START);
    for(UC_INT32 nIndex = WEB_TAG_TYPE_W3C_START; nIndex < WEB_TAG_TYPE_W3C_END; nIndex++)
    {
        pInfo = debug_getTagInfo(nIndex);
        UC_INT32 nTagToken = getTagToken(pInfo->s_name);
        if(pInfo->eTagType_m != nTagToken)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(pInfo->eTagType_m == nTagToken);
    }

    CPPUNIT_ASSERT(-1 == getTagToken(NULL));
    CPPUNIT_ASSERT(-1 == getTagToken("adfkdfeijsldkfjlksd"));
}

void UcWebTagInfoTest_c::testGetAttributeIndex()
{
    //const Wap2Keyword* getAttriInfo(const UC_CHAR* sAttriName, UC_INT32 nLen);
    const UcWebAttributeInfo_t* pInfo = debug_getAttriInfo(WEB_ATTRIBUTE_TYPE_ABBR);
    for(UC_INT32 nIndex = WEB_ATTRIBUTE_TYPE_ABBR; nIndex < WEB_ATTRIBUTE_TYPE_END; nIndex++)
    {
        pInfo = debug_getAttriInfo(nIndex);
        UC_INT32 nAttriToken = getAttributeToken(pInfo->s_name);
        if(pInfo->eAttrType_m != nAttriToken)
        {
            int sd = 0;
        }
        CPPUNIT_ASSERT(pInfo->eAttrType_m == nAttriToken);
    }
    CPPUNIT_ASSERT(-1 == getAttributeToken(NULL));
    CPPUNIT_ASSERT(-1 == getAttributeToken("adfkdfeijsldkfjlksd"));
}
