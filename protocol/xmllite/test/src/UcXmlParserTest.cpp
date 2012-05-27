////////////////////////////////////////////////////////////////////////////////
// File Name:      UcXmlParserTest.cpp                              
// Author:          Yangxm                                                         
// Date:             2011-06-09                                         
// Copyright:      2011 UC, All Rights Reserved.                            
// Description:    
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2011/06/09    Yangxm        Initial creation.                                  
// -------------------------------------------------------------------------
//                                                                            
////////////////////////////////////////////////////////////////////////////////
#include "protocol/xmllite/test/inc/UcXmlParserTest.h"
#include "protocol/xmllite/test/inc/UcXmlParserTestData.h"
#include "third_party/cppunit/fm/inc/UcCppUnitFM.h"


#include "protocol/xmllite/inc/UcXmlParserWapper.h"
#include "stubs/inc/ReadFileBytes.h"

extern int nStep = 0;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(UcXmlParserTest, "testXmlParser");

void StartElementHandler_stub_CPPUNIT(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    //printf("start element\n");
    printf("element : %s\n", paEle->piElement->name);
    if(paEle->iAttriNum > 0)
    {
        char str[500];
        for(int i = 0; i < paEle->iAttriNum; i++)
        {
            printf("element attribute : %s\n", paEle->iAttriLst[i].piAttri->name);
            memcpy(str, paEle->iAttriLst[i].piAttriValue, paEle->iAttriLst[i].iAttriValueLen);
            str[paEle->iAttriLst[i].iAttriValueLen] = '\0';
            printf("element attribute value : %s\n", str);
        }
    }
}

void EndElementHandler_stub_CPPUNIT(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
    //printf("end element\n");
}
void WMLCharDataHandler_stub_CPPUNIT(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
    //printf("data callback");
}

UcXmlParserTest::UcXmlParserTest() :
    pXmlParser_m(NULL)
{
}

UcXmlParserTest::~UcXmlParserTest()
{
	if (NULL != pXmlParser_m)
	{
		delete pXmlParser_m;
		pXmlParser_m = NULL;
	}
}

void UcXmlParserTest::setUp()
{
    if (NULL == pXmlParser_m)
    {
        pXmlParser_m = new UcXmlParserWapper_c();

    }
	
}
void UcXmlParserTest::tearDown()
{
	if (NULL != pXmlParser_m)
	{
		delete pXmlParser_m;
		pXmlParser_m = NULL;
	}
}

void UcXmlParserTest::testSimpleParsing()
{
    void* pD = NULL;

    int nLen = read_all_bytes("..\\..\\..\\test\\test_pages\\lp_navi.xml", &pD);

    pXmlParser_m->parseData((const char*)pD, nLen, true);

    free_all_bytes(pD);
}

void UcXmlParserTest::testhistoryParsing()
{ 
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_history,
                &UcXmlParserTest::EndElementHandler_history,
                &UcXmlParserTest::WMLCharDataHandler_history);
    }

    nStep = 0;

    const char* pD = historyData;

    int nLen = sizeof(historyData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_history(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "history", strlen("history")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "title", strlen("title")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "title", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "date", strlen("date")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "123", paEle->iAttriLst[1].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttri->name, "href", strlen("href")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttriValue, "href", paEle->iAttriLst[2].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[3].piAttri->name, "link_type", strlen("link_type")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[3].piAttriValue, "web", paEle->iAttriLst[3].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[4].piAttri->name, "icon_src", strlen("icon_src")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[4].piAttriValue, "icon", paEle->iAttriLst[4].iAttriValueLen));
    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_history(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_history(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}


void UcXmlParserTest::testpreferenceParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_preference,
                &UcXmlParserTest::EndElementHandler_preference,
                &UcXmlParserTest::WMLCharDataHandler_preference);
    }

    nStep = 0;

    const char* pD = preferenceData;

    int nLen = sizeof(preferenceData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_preference(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{ 
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "preference", strlen("preference")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "dir", strlen("dir")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "title", strlen("title")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "title", paEle->iAttriLst[0].iAttriValueLen));
    }
    else if(2 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "title", strlen("title")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "title", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "href", strlen("href")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "href", paEle->iAttriLst[1].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttri->name, "link_type", strlen("link_type")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttriValue, "wap", paEle->iAttriLst[2].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[3].piAttri->name, "icon_src", strlen("icon_src")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[3].piAttriValue, "icon", paEle->iAttriLst[3].iAttriValueLen));
    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_preference(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_preference(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testsysconfmapParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_sysconfmap,
                &UcXmlParserTest::EndElementHandler_sysconfmap,
                &UcXmlParserTest::WMLCharDataHandler_sysconfmap);
    }

    nStep = 0;

    const char* pD = sysconfmapData;

    int nLen = sizeof(sysconfmapData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_sysconfmap(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "sys_conf_map", strlen("sys_conf_map")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "group", strlen("group")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "title", strlen("title")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "title", paEle->iAttriLst[0].iAttriValueLen));
    }
    else if(2 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "id", strlen("id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "1", paEle->iAttriLst[0].iAttriValueLen));
    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_sysconfmap(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_sysconfmap(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testsysinfoParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_sysinfo,
                &UcXmlParserTest::EndElementHandler_sysinfo,
                &UcXmlParserTest::WMLCharDataHandler_sysinfo);
    }

    nStep = 0;

    const char* pD = sysinfoData;

    int nLen = sizeof(sysinfoData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_sysinfo(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "sys_info", strlen("sys_info")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "info", strlen("info")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "id", strlen("id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "123", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "title", strlen("title")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "title", paEle->iAttriLst[1].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttri->name, "value", strlen("value")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttriValue, "true", paEle->iAttriLst[2].iAttriValueLen));
    }
    else if(2 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "option", strlen("option")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "name", strlen("name")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "name", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "value", strlen("value")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "value", paEle->iAttriLst[1].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttri->name, "selected", strlen("selected")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttriValue, "true", paEle->iAttriLst[2].iAttriValueLen));

    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_sysinfo(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_sysinfo(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testsysconfParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_sysconf,
                &UcXmlParserTest::EndElementHandler_sysconf,
                &UcXmlParserTest::WMLCharDataHandler_sysconf);
    }

    nStep = 0;

    const char* pD = sysconfData;

    int nLen = sizeof(sysconfData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_sysconf(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "sys_conf", strlen("sys_conf")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "id", strlen("id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "1", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "value", strlen("value")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "value", paEle->iAttriLst[1].iAttriValueLen));

    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_sysconf(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_sysconf(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testtabbarParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_tabbar,
                &UcXmlParserTest::EndElementHandler_tabbar,
                &UcXmlParserTest::WMLCharDataHandler_tabbar);
    }

    nStep = 0;

    const char* pD = tabbarData;

    int nLen = sizeof(tabbarData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_tabbar(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "ucf", strlen("ucf")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "type", strlen("type")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "tab", paEle->iAttriLst[0].iAttriValueLen));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "ucf", strlen("ucf")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "type", strlen("type")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "tab_item", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "name", strlen("name")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "name", paEle->iAttriLst[1].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttri->name, "value", strlen("value")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttriValue, "value", paEle->iAttriLst[2].iAttriValueLen));

    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_tabbar(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_tabbar(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testmenuParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_menu,
                &UcXmlParserTest::EndElementHandler_menu,
                &UcXmlParserTest::WMLCharDataHandler_menu);
    }

    nStep = 0;

    const char* pD = menuData;

    int nLen = sizeof(menuData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_menu(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "menu", strlen("menu")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "group", strlen("group")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "id", strlen("id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "123", paEle->iAttriLst[0].iAttriValueLen));

    }
    else if(2 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "title", strlen("title")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "title", paEle->iAttriLst[0].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttri->name, "accessory", strlen("accessory")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[1].piAttriValue, "#8", paEle->iAttriLst[1].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttri->name, "event_id", strlen("event_id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[2].piAttriValue, "1", paEle->iAttriLst[2].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[3].piAttri->name, "type", strlen("type")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[3].piAttriValue, "check", paEle->iAttriLst[3].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[4].piAttri->name, "sub_menu_id", strlen("sub_menu_id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[4].piAttriValue, "1", paEle->iAttriLst[4].iAttriValueLen));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[5].piAttri->name, "sys_conf_id", strlen("sys_conf_id")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[5].piAttriValue, "1", paEle->iAttriLst[5].iAttriValueLen));
    }


    nStep++;
}

void UcXmlParserTest::EndElementHandler_menu(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_menu(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testsearchhistoryParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_searchhistory,
                &UcXmlParserTest::EndElementHandler_searchhistory,
                &UcXmlParserTest::WMLCharDataHandler_searchhistory);
    }

    nStep = 0;

    const char* pD = searchhistoryData;

    int nLen = sizeof(searchhistoryData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_searchhistory(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "searchhistory", strlen("searchhistory")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "value", strlen("value")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "value", paEle->iAttriLst[0].iAttriValueLen));

    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_searchhistory(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_searchhistory(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void UcXmlParserTest::testurlhistoryParsing()
{
    if (NULL != pXmlParser_m && pXmlParser_m->init())
    {
        pXmlParser_m->setCallBacks(this,
                &UcXmlParserTest::StartElementHandler_urlhistory,
                &UcXmlParserTest::EndElementHandler_urlhistory,
                &UcXmlParserTest::WMLCharDataHandler_urlhistory);
    }

    nStep = 0;

    const char* pD = urlhistoryData;

    int nLen = sizeof(urlhistoryData) - 1;

    pXmlParser_m->parseData((const char*)pD, nLen, true);
}

void UcXmlParserTest::StartElementHandler_urlhistory(void* userData,const WAPElement *paEle, UC_INT nIsEmpty)
{
    if(0 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "urlhistory", strlen("urlhistory")));
    }
    else if(1 == nStep)
    {
        CPPUNIT_ASSERT_MESSAGE("element error", 0 == strncmp((const char *)paEle->piElement->name, "item", strlen("item")));

        CPPUNIT_ASSERT_MESSAGE("element attribute error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttri->name, "value", strlen("value")));
        CPPUNIT_ASSERT_MESSAGE("element attribute value error", 0 == strncmp((const char *)paEle->iAttriLst[0].piAttriValue, "value", paEle->iAttriLst[0].iAttriValueLen));

    }

    nStep++;
}

void UcXmlParserTest::EndElementHandler_urlhistory(void *handler, UC_INT token, UC_BOOL isrealEnd)
{
}

void UcXmlParserTest::WMLCharDataHandler_urlhistory(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag)
{
}

void runXmlParserTest()
{
     runTestCase("testXmlParser","xmlParserTestResult.log");
}

