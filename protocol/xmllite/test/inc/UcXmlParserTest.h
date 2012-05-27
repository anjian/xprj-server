////////////////////////////////////////////////////////////////////////////////
// File Name:      UcXmlParserTest.h                              
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
#ifndef __UC_XML_PARSER_TEST_H__
#define __UC_XML_PARSER_TEST_H__

//#include "third_party/cppunit/include/cppunit/extensions/HelperMacros.h"

#include "third_party/cppunit/fm/inc/UcCppUnitFM.h"
#include "platform/inc/ucweb_os_type.h"
#include "interface/protocol/UcXmlParserIf.h"

extern void runXmlParserTest();

class UcXmlParserWapper_c;

class UcXmlParserTest  : public CppUnit::TestFixture
{
 CPPUNIT_TEST_SUITE( UcXmlParserTest );
    //Add new test cases here.
    CPPUNIT_TEST(testhistoryParsing);
    CPPUNIT_TEST(testpreferenceParsing);
    CPPUNIT_TEST(testsysconfmapParsing);
    CPPUNIT_TEST(testsysinfoParsing);
    CPPUNIT_TEST(testsysconfParsing);
    CPPUNIT_TEST(testtabbarParsing);
    CPPUNIT_TEST(testmenuParsing);
    CPPUNIT_TEST(testsearchhistoryParsing);
    CPPUNIT_TEST(testurlhistoryParsing);
 CPPUNIT_TEST_SUITE_END();
	
 public:
       UcXmlParserTest();
       ~UcXmlParserTest();
	   
	void setUp();
	void tearDown();

private:
	void testSimpleParsing();
	void testhistoryParsing();
	void testpreferenceParsing();
	void testsysconfmapParsing();
	void testsysinfoParsing();
	void testsysconfParsing();
	void testtabbarParsing();
	void testmenuParsing();
	void testsearchhistoryParsing();
	void testurlhistoryParsing();
	//TODO: more TCs to be added
    
    static void StartElementHandler_history(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_history(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_history(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_preference(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_preference(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_preference(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_sysconfmap(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_sysconfmap(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_sysconfmap(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_sysinfo(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_sysinfo(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_sysinfo(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_sysconf(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_sysconf(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_sysconf(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_tabbar(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_tabbar(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_tabbar(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_menu(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_menu(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_menu(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_searchhistory(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_searchhistory(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_searchhistory(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);

    static void StartElementHandler_urlhistory(void* userData,const WAPElement *paEle, UC_INT nIsEmpty);
    static void EndElementHandler_urlhistory(void *handler, UC_INT token, UC_BOOL isrealEnd);
    static void WMLCharDataHandler_urlhistory(void* userData,const UC_BYTE *s, UC_INT len, UC_INT isDollarFlag);
private:
	UcXmlParserWapper_c*   pXmlParser_m;
		 
};
#endif
