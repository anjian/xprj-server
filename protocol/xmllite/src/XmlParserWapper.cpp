////////////////////////////////////////////////////////////////////////////////
// File Name:      UcXmlParserWapper.cpp                              
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
////////////////////////////////////////////////////////////////////////////////


#include <interface/protocol/UcWebTagInfo.h>

#include "protocol/xmllite/inc/UcXmlParserWapper.h"

UcXmlParserWapper_c::UcXmlParserWapper_c():
    pXmlParser_m(NULL),
    pUser_m(NULL),
    fnStartTagHandler_m(NULL),
    fnEndTagHandler_m(NULL),
    fnCharacterHandler_m(NULL),
    elementStack_m(6)
{
}
UcXmlParserWapper_c::~UcXmlParserWapper_c()
{
    if (pXmlParser_m)
    {
        WmlSaxParserDelete(&pXmlParser_m);
        delete pXmlParser_m;
    }
}
UC_BOOL UcXmlParserWapper_c::init()
{
     pXmlParser_m = WmlSaxParserNew();	
     if (NULL == pXmlParser_m)
     {
         return UC_FALSE;
     }
     WmlSaxParserSetEventHandler(pXmlParser_m,
         (void*)this,												 
         startElementHandler_CB,
         endElementHandler_CB,
         charactersHandler_CB);

     return UC_TRUE;
}
void UcXmlParserWapper_c::handleStartElement(const WAPElement *pElement,UC_INT nIsEmpty)
{
    if (NULL != pElement)
    {
        switch (pElement->piElement->token)
        {
            case WEB_TAG_TYPE_IMG:
            case WEB_TAG_TYPE_INPUT:
            case WEB_TAG_TYPE_BR:
            case WEB_TAG_TYPE_HR:
                {
                    // self close tag, don't put it into stack
                    fnStartTagHandler_m(pUser_m, pElement, nIsEmpty);
                    fnEndTagHandler_m(pUser_m, pElement->piElement->token, TAG_COMPLETE_NORMAL);
                }
                break;
            default:
                elementStack_m.append(pElement->piElement->token);
                fnStartTagHandler_m(pUser_m,pElement,nIsEmpty);
                break;
        }

        if (1 == nIsEmpty)
        {
            handleEndElement(pElement);
        }
    }
}
XMLErrorTYpe UcXmlParserWapper_c::parseData(const UC_CHAR* sBuf,UC_INT32 nDataLen,UC_BOOL isFinish)
{
    return WmlSaxParserParseBuf(pXmlParser_m,(UC_BYTE*)sBuf,nDataLen,isFinish);
}
UC_BOOL UcXmlParserWapper_c::inStack(UC_INT nToken)
{
    for (UC_INT i = elementStack_m.size() -1 ; i >= 0 ; i--)
    {
        if (nToken == elementStack_m.get(i))
        {
            return UC_TRUE;
        }
    }
    return UC_FALSE;
}
void UcXmlParserWapper_c::handleEndElement(const WAPElement *pElement)
{
    if (NULL == pElement)
    {
        return;
    }

    switch (pElement->piElement->token)
    {
        case WEB_TAG_TYPE_IMG:
        case WEB_TAG_TYPE_INPUT:
        case WEB_TAG_TYPE_BR:
        case WEB_TAG_TYPE_HR:
            {
                // self close tag, ignore end element
                return;
            }
            break;
        default:;
    }

//#define IS_NOT_REAL_END  UC_FALSE
//#define IS_REAL_END      UC_TRUE
    if (elementStack_m.size() <= 0)
    {
        return;
    }

    UC_INT nTopNode = elementStack_m.getLast();

    if ( nTopNode == pElement->piElement->token)
    {
        //The coming tag matches the top tag, pop it.
        fnEndTagHandler_m(pUser_m,nTopNode,TAG_COMPLETE_NORMAL);
        elementStack_m.takeLast(); 

        return;
    }
    else if (UC_FALSE == inStack(pElement->piElement->token))
    {
        if (pElement->piElement->token == WEB_TAG_TYPE_BODY || pElement->piElement->token == WEB_TAG_TYPE_CARD)
        {
            fnEndTagHandler_m(pUser_m,pElement->piElement->token,TAG_COMPLETE_NOTSTART);
        }
        //The stack is empty or the tag is not in the stack.
        //Let the model knows anyway.
        // received end tag doesn't exit in stack, discard it silently
        return;
    }
    else
    //if ( nTopNode != pElement->piElement->token)
    {
        // Anyway, pop-up the first mismatched tag
        fnEndTagHandler_m(pUser_m,nTopNode,TAG_COMPLETE_FALLBACK);
        elementStack_m.takeLast();

        // try to find the correct one: until find the half of stack
        {
            UC_INT32 popCount   = (elementStack_m.size() >> 1);
            XML_TagCompleteType_e isRealEnd   = TAG_COMPLETE_NORMAL;
            do
            {
                nTopNode    = elementStack_m.getLast(); 
                isRealEnd   = (nTopNode == pElement->piElement->token) ? TAG_COMPLETE_NORMAL : TAG_COMPLETE_FALLBACK;

                popCount--;

                fnEndTagHandler_m(pUser_m,nTopNode,isRealEnd);
                elementStack_m.takeLast(); 
            } while ((popCount > 0) && (!isRealEnd));
        }
    }
}
void UcXmlParserWapper_c::handleCharacters(const UC_BYTE *sBuf, const UC_INT nLen, UC_INT nDollarFlag)
{
    fnCharacterHandler_m(pUser_m,sBuf,nLen,nDollarFlag);
}
void UcXmlParserWapper_c::setCallBacks(void* pUser,
                  WMLStartElementHandlerFunc startCallback,
                  WMLEndElementHandlerFunc4WebData endCallback,
                  WMLCharacterDataHandlerFunc charCallback)
{
     pUser_m = pUser;
     fnStartTagHandler_m = startCallback;
     fnEndTagHandler_m = endCallback;
     fnCharacterHandler_m = charCallback;
}
void startElementHandler_CB(void *handler, const WAPElement * paEle, UC_INT bIsEmpty)
{
    UcXmlParserWapper_c * pParserWapper = (UcXmlParserWapper_c*)handler;
    if (pParserWapper)
    {
        pParserWapper->handleStartElement(paEle,bIsEmpty);
    }
}

void endElementHandler_CB(void *handler, const WAPElement *paEle)
{

    UcXmlParserWapper_c * pParserWapper = (UcXmlParserWapper_c*)handler;
    if (pParserWapper)
    {
        pParserWapper->handleEndElement(paEle);
    }
}

void charactersHandler_CB(void *handler, const UC_BYTE *sBuf, const UC_INT nLen, UC_INT nDollarFlag)
{
    UcXmlParserWapper_c * pParserWapper = (UcXmlParserWapper_c*)handler;
    if (pParserWapper)
    {
        pParserWapper->handleCharacters(sBuf,nLen,nDollarFlag);
    }
}
