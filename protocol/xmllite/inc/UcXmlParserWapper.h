////////////////////////////////////////////////////////////////////////////////
// File Name:      UcXmlParserWapper.h                              
// Author:                                                                  
// Date:           2011-3-31                                         
// Copyright:      2011 UC, All Rights Reserved.                            
// Description:     
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2011/05/06    Yangxm        Initial creation.                                  
// -------------------------------------------------------------------------
//                                                                            
///////////////////////////////////////////////////////////////////////////////
#ifndef __UC_XML_PARSER_H__
#define __UC_XML_PARSER_H__

//#include <platform/inc/ucweb_os_type.h>
#include <com/inc/UcContainer.h>

#include <interface/protocol/UcXmlParserIf.h>

#include "protocol/xmllite/inc/WMLSaxParser.h"

class UcXmlParserWapper_c
{
    public:
        UcXmlParserWapper_c();
        ~UcXmlParserWapper_c();
        UC_BOOL init();
        XMLErrorTYpe parseData(const UC_CHAR* sBuf,UC_INT32 nDataLen,UC_BOOL isFinish);

        void handleStartElement(const WAPElement *paEle,UC_INT aIsEmpty);
        void handleEndElement(const WAPElement *paEle);
        void handleCharacters(const UC_BYTE *sBuf, const UC_INT nLen, UC_INT nDollarFlag);
        void setCallBacks(void* pUser,
                WMLStartElementHandlerFunc startCallback,
                WMLEndElementHandlerFunc4WebData endCallback,
                WMLCharacterDataHandlerFunc charCallback);
    private:
        UC_BOOL inStack(UC_INT nToken);

    private:
        WMLSaxParser* pXmlParser_m;
        void*   pUser_m;
        WMLStartElementHandlerFunc fnStartTagHandler_m;
        WMLEndElementHandlerFunc4WebData fnEndTagHandler_m;
        WMLCharacterDataHandlerFunc fnCharacterHandler_m;

        UcTempSimpleVector_c<UC_INT> elementStack_m;
};

void startElementHandler_CB(void *handler, const WAPElement * paEle, UC_INT bIsEmpty);
void endElementHandler_CB(void *handler, const WAPElement *paEle);
void charactersHandler_CB(void *handler, const UC_BYTE *sBuf, const UC_INT nLen, UC_INT nDollarFlag);

#endif
