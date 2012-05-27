////////////////////////////////////////////////////////////////////////////////
// File Name:      UcWebTagInfoTest.h                              
// Author:         Luobin
// Date:           2011-10-29                                         
// Copyright:      2011 UC, All Rights Reserved.                            
// Description:    
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2011/10/29    Luobin        Initial creation.                                  
// -------------------------------------------------------------------------
//                                                                            
////////////////////////////////////////////////////////////////////////////////
#ifndef __UC_WEB_TAG_INFO_TEST_H
#define __UC_WEB_TAG_INFO_TEST_H

#include "third_party/cppunit/fm/inc/UcCppUnitFM.h"
#include "platform/inc/ucweb_os_type.h"

extern void runWebTagInfoTest();

class UcWebTagInfoTest_c  : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE(UcWebTagInfoTest_c);
        //Add new test cases here.
        CPPUNIT_TEST(testTagCompleteness);
        CPPUNIT_TEST(testAtrributeCompleteness);
        CPPUNIT_TEST(testTagArrayIsAscendingAlphabeticOrder);
        CPPUNIT_TEST(testAttribArrayIsAscendingAlphabeticOrder);
        CPPUNIT_TEST(testGetTagInfo);
        CPPUNIT_TEST(testGetAttriInfo);
        CPPUNIT_TEST(testGetTagName);
        CPPUNIT_TEST(testGetAttriName);
        CPPUNIT_TEST(testGetTagIndex);
        CPPUNIT_TEST(testGetAttributeIndex);

        CPPUNIT_TEST_SUITE_END();

    public:
        UcWebTagInfoTest_c()                {}
        ~UcWebTagInfoTest_c()               {}

        void setUp();
        void tearDown();

    private:
        //verify whether all enum is mapping
        void testTagCompleteness();
        void testAtrributeCompleteness();

        void testTagArrayIsAscendingAlphabeticOrder(); 
        void testAttribArrayIsAscendingAlphabeticOrder(); 

        //interface test
        void testGetTagInfo();
        void testGetAttriInfo();
        void testGetTagName();
        void testGetAttriName();
        void testGetTagIndex();
        void testGetAttributeIndex();
};

#endif
