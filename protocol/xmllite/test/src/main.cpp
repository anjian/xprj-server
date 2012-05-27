#include "protocol/xmllite/test/inc/UcXmlParserTest.h"
#include "protocol/xmllite/test/inc/UcWebTagInfoTest.h"
#include "interface/uc_memory.h"

#define  PROTOCOL_CPPUNIT_ENABLE  1
#include "stubs/inc/ReadFileBytes.h"

#include<stdio.h>

void dump_log_file(const char * sFileName)
{
    char* sFileContent = NULL;
    int nFileLen = read_all_bytes(sFileName, (void**)&sFileContent);
    printf("===========[%s]=====================================\n", sFileName);
    if(sFileContent)
    {
        char* sContent = new char[nFileLen + 1];
        memcpy(sContent, sFileContent, nFileLen);
        sContent[nFileLen] = '\0';
        free_all_bytes(sFileContent);
        printf("%s\n", sContent);
        delete[] sContent;
    }
    printf("===========================================================\n");
}
int main(int argc, char** argv)
{
    uc_mem_init();

    runXmlParserTest();
    dump_log_file("xmlParserTestResult.log");

    runWebTagInfoTest();
    dump_log_file("WebTagInfo.log");

    uc_memdump();

	return 0;
}
