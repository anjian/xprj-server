/*******************************************************************************
* Copyright (C) 2004 - 2010 UCWEB Technology Ltd.
* All rights reserved.
* 文件名:           WMLSaxParser.c  
* 作者:             danwl
* 版本:             $Revision$
* 创建日期:         2010-07-16
* 所属项目：		UcWeb客户端
* 模块描述:         基础公共
* 版本信息:         暂无
* 平台：			MTK
* 主要函数及其功能:
*					
* 记录:
// yyyymmmdd Changed by      Change Reason
// --------- --------------- --------------------------------------------------
// 2010Dec02 Yangxm          Change for Brew7.2 
// 2011Jan25 Yangxm          M:0058510 - Add UTF-8 finished detection.
// 2011Jan26 Yangxm          Add variable enclosing detection. 
*******************************************************************************/

//#include "UtilityLib.h"
#include "protocol/xmllite/inc/WMLSaxParser.h"
//#include "protocol/xmllite/inc/attributeListAUTO.h"
//#include "protocol/xmllite/inc/elementListAUTO.h"

#include "interface/protocol/UcWebTagInfo.h"

#ifdef  PROTOCOL_CPPUNIT_ENABLE
#include "third_party/cppunit/fm/inc/UcStdLibrary_stub.h"
#include "third_party/cppunit/fm/inc/uc_memory_stub.h"
#else
#include "com/inc/UcStdLibrary.h"
#include "interface/uc_memory.h"
#endif
//#include "configProxy.h"


/*
 * =============================================================================
 *  宏定义区
 * =============================================================================
 */
#define HEX_MASK							0x1
#define FIRST_NAME_MASK						0x2
#define DECIMAL_MASK						0x4
#define TEXT_END_MASK						0x8
#define WEB_TAG_TYPE_END_MASK					0x10
#define ATTRIBUTE_END_MASK					0x20
#define VALUE_END_MASK						0x40
#define SLASH_SPACE_MASK					0x80


static const UC_BYTE S_CharFlagTable[] =
{
    0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, //16
    0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, //32
    0xf8, 0x0,  0x0,  0x0,  0x8,  0x0,  0x8,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0xb0, 
    0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x5,  0x0,  0x0,  0x8,  0x20, 0x78, 0x0, 
    0x0,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2, 
    0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x3,  0x3,  0x3,  0x3,  0x3,  0x3,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2, 
    0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x2,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0, 0x0
}; 

#define  XML_SUPPORT_ENTITY_NUM		16		
static const Entity S_EntityTable[] =            
{  
    //{ascii code, len, name}
    {8221,    6,   "rdquo;"  },
    {0x00BB,  6,   "raquo;"  },
    {8220,    6,   "ldquo;"  },
    {183,     7,   "middot;" },
    {169,     5,   "copy;"   },
    {174,     4,   "reg;"    },
    {45,      4,   "shy;"    },
    {62,      3,   "gt;"     },
    {60,      3,   "lt;"     },
    {39,      5,   "apos;"   },
    {34,      5,   "quot;"   },
    {38,      4,   "amp;"    },
    {32,      5,   "nbsp;"   }
};

static UC_INT S_EntityTableSZ = sizeof(S_EntityTable)/sizeof(Entity);



#define IsCharSpace(c)					((c) <= ' ')

#define IsCharHex(c)					((S_CharFlagTable[(UC_BYTE)(c)] & HEX_MASK) != 0)
#define IsCharDecimal(c)				((S_CharFlagTable[(UC_BYTE)(c)] & DECIMAL_MASK) != 0)

#define IsCharSlashSpace(c)				((S_CharFlagTable[(UC_BYTE)(c)] & SLASH_SPACE_MASK) != 0)
#define IsCharFirstName(c)				((S_CharFlagTable[(UC_BYTE)(c)] & FIRST_NAME_MASK) != 0)
#define IsCharNameEnd(c)				((S_CharFlagTable[(UC_BYTE)(c)] & WEB_TAG_TYPE_END_MASK) != 0)
#define IsCharAttriNameEnd(c)			((S_CharFlagTable[(UC_BYTE)(c)] & ATTRIBUTE_END_MASK) != 0)
#define IsCharValueEnd(c)				((S_CharFlagTable[(UC_BYTE)(c)] & VALUE_END_MASK) != 0)
#define IsCharTextEnd(c)				((S_CharFlagTable[(UC_BYTE)(c)] & TEXT_END_MASK) != 0)

#define peek()							((*psPeeker))
#define peekn(n)						(*(psPeeker + (n)))

#define consume()						(psPeeker++)
#define consumen(n)						(psPeeker += (n))

#define SkipSlashSpace()				{for (; IsCharSlashSpace(peek()); consume());}
#define SkipWhiteSpace()				{for (; IsCharSpace(peek()); consume());}
#define SkipToRightAngle()				{for (; *psPeeker != '>'; psPeeker++);}
#define PushChar(c)						(pself->piTextBuf[pself->iTextPos++] = (UC_BYTE)(c))

#define SenseElementName(s)				{for ((s) = psPeeker; !IsCharNameEnd(*(s)); (s)++);}
//#define ConsumeWhiteSpace(s)			{for ((s) = psPeeker; IsCharSpace(*psPeeker); consume());}
#define ScanAttributeValue(s)			{for ((s) = psPeeker; (s) <= psParseBufEnd && sDilimiter != *(s); (s)++);}
#define ScanAttributeName(s)			{for((s) = psPeeker; !IsCharAttriNameEnd(*(s)); (s)++);}
#define SenseAttributeValue(s)			{for ((s) = psPeeker; !IsCharValueEnd(*(s)); (s)++);}


#define INITIAL_BUF_SIZE					512
#define INITIAL_BUF_SIZE_LITTLE				8


UC_BYTE upper2lower(UC_BYTE a)
{
    if ( (a >= 'A') && (a <= 'Z'))
    {
        return a+32;
    }
    else
    {
        return a;
    }
}

/*****************************************************************************
 * FUNCTION
 *  ucs2_to_utf8
 * DESCRIPTION
 *  
 * PARAMETERS
 *  utf8        [?]         
 *  ucs2        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
UC_INT unicode_to_utf8(UC_BYTE* utf8, UC_WORD ucs2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    UC_INT count;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (ucs2 < 0x80)
    {
        count = 1;
    }
    else if (ucs2 < 0x800)
    {
        count = 2;
    }
    else
    {
        count = 3;
    }
    switch (count)
    {
        case 3:
            utf8[2] = 0x80 | (ucs2 & 0x3f);
            ucs2 = ucs2 >> 6;
            ucs2 |= 0x800;
        case 2:
            utf8[1] = 0x80 | (ucs2 & 0x3f);
            ucs2 = ucs2 >> 6;
            ucs2 |= 0xc0;
        case 1:
            utf8[0] = (UC_UINT8) ucs2;
    }
    return count;
}


/*
* =============================================================================
*  局部函数实现区
* =============================================================================
*/
static UC_INT 
TextBufGrow(WMLSaxParser * pself);

static void
PushText(WMLSaxParser *pself, UC_BYTE c)
{
	 if (pself->iTextPos < pself->iTextBufLen)
	 {
		 PushChar(c);
	 }
	 else if (TextBufGrow(pself))
	 {
		 PushChar(c);
	 }
}

UC_INT GetCountOf1Ahead(UC_BYTE sData)
{
    UC_INT sRet = 0;
    UC_INT i;

    for(i = 0 ; i < 4 ; ++i,sData<<= 1 )
    {
        if(sData & 0x80)
        {
            sRet++;
        }
        else
        {
            break;
        }
    }
    return sRet;
}

UC_INT getStartByteOfUTF8(UC_BYTE* buf,UC_INT index)
{
    UC_INT sCount;
    UC_INT i;
    
    for (i = 0; i < 4 && (index-i)>=0; i++)
    {
        sCount = GetCountOf1Ahead(buf[index-i]);
        if (i==0) 
        {
            if (sCount == 0)
            {
                return 0;  //It is the start of 1byte utf-8.
            }
            else if (sCount > 1)
            {
                return 1;////It is the start of nbyte utf-8.
            }
        }
        else
        {
            if(sCount == i + 1)
            {
                return 0; //The i+1 bytes UTF-8 finished.
            }
        }

        if (sCount >= (i+2))
        {
           //Not finished.Return the index of the start of this utf8.
            return i+1;
        }
    }
    return 0;
}

void SendCharacter(WMLSaxParser* pself, UC_BOOL isForceOut)
{
    UC_INT index;
    UC_INT i;
    UC_INT leftLen;
    UC_INT sCount;
    UC_BOOL hasDollarFlag;

    // check tailer space
    if ((pself->bFilterTailSpace_m) && (IsCharSpace(pself->piTextBuf[pself->iTextPos -1])))
    {
        pself->iTextPos--;
        pself->bFilterTailSpace_m = UC_FALSE;

        if (pself->iTextPos <= 0)
        {
            return;
        }
    }

    if (pself->iDollarFlag == VARIABLE_ENCLOSED
        || pself->iDollarFlag == NO_DOLLAR_OCCURRED)
    {
        hasDollarFlag = (pself->iDollarFlag == VARIABLE_ENCLOSED)?1:0;
        if (isForceOut)
        {
            // check whether the last character is finished utf-8 character, if not, keep it for next sending out
            // If VARIABLE_ENCLOSED,even rollback the unfinished utf-8 bytes,it will not cross the variable.
            sCount = getStartByteOfUTF8(pself->piTextBuf,pself->iTextPos-1);
            index = pself->iTextPos-sCount;

            pself->piCharacterFunc(pself->piEventHander, pself->piTextBuf,index ,hasDollarFlag);

            //Move the left to the start of the text buffer
            for (i = 0; i< sCount;i++)
                pself->piTextBuf[i] = pself->piTextBuf[index + i];
            pself->iTextPos = sCount;
           
        }
        else
        { //The normal output, no need to check the utf8 finished or not.Just send it out.
            pself->piCharacterFunc(pself->piEventHander, pself->piTextBuf,pself->iTextPos, hasDollarFlag);
            pself->iTextPos = 0;
        }
        pself->iDollarFlag = NO_DOLLAR_OCCURRED;
    }
    else if (pself->iDollarFlag == VARIABLE_STARTED) //The variable not enclosed,
    {
        index = pself->iTextPos -1;
        while (index>=0 && pself->piTextBuf[index] != '$')
        {
            index--;
        }
        pself->piCharacterFunc(pself->piEventHander, pself->piTextBuf,index, UC_FALSE);
        //Keep it for next sending.
        leftLen = pself->iTextPos - index;
        for (i = 0; i <leftLen; i++)
        {
            pself->piTextBuf[i] = pself->piTextBuf[index+i];
        }
        pself->iTextPos = leftLen;
    }
}

/***************************************************************************
说明:   增大文本缓冲buf的长度，当增大到阀值或内存不足后不再继续增大而是将文本外抛
输入:	pself WMLSaxParser指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static UC_INT 
TextBufGrow(WMLSaxParser * pself)		
{
	UC_BYTE *sNewBuf;

	if (pself->iTextBufLen <= (TEXT_BUF_MAX_LEN >> 1) &&
		NULL != (sNewBuf = (UC_BYTE *)ucmalloc((pself->iTextBufLen << 1) + MALLOC_EXTRA_SIZE)))
	{
		uc_memcpy(sNewBuf, pself->piTextBuf, pself->iTextBufLen);
		ucfree(pself->piTextBuf);
		pself->piTextBuf = sNewBuf;
		pself->iTextBufLen <<= 1;
	}
	else 
	{
      SendCharacter(pself,UC_TRUE);//UC_TRUE indicates force out, need to check the utf8 finished or not.
	}
	
	return UC_TRUE;
}

void ResetBuffer(WMLSaxParser* pself,UC_BYTE* startPos,UC_INT offset, UC_INT len) 
{
    UC_INT sRemainLen = pself->piLeftBufEnd - pself->piLeftBuf;    
    //Find the '>', if not found, set the pointer that can't be processed.
    pself->piLeftBuf = (UC_BYTE *)uc_strrchr((UC_PSTRING)(startPos + offset), '>');
    if (!pself->piLeftBuf)
    {
        pself->piLeftBuf = startPos;
        pself->piParseBufEnd = pself->piLeftBuf;
    }
    else
    {
        pself->piParseBufEnd = pself->piLeftBuf;
        pself->piLeftBuf++;
    }

    pself->piLeftBufEnd = startPos + sRemainLen + len;
    pself->piParseBuf = startPos;

}
UC_BOOL CreateNewBuffer(WMLSaxParser *pself, UC_BYTE *buf,UC_INT len,UC_INT sNeedsz, UC_INT sRemainSz)
{
    UC_UINT sMallocSz = sNeedsz + INITIAL_BUF_SIZE;
    UC_BYTE *sNewBuf = (UC_BYTE *)ucmalloc(sMallocSz + MALLOC_EXTRA_SIZE); //用于最后的存放'\0'的空间

    if (!sNewBuf)
    {
        sMallocSz = sNeedsz + INITIAL_BUF_SIZE_LITTLE;
        if (NULL == (sNewBuf = (UC_BYTE *)ucmalloc(sMallocSz + MALLOC_EXTRA_SIZE))) // memory notice
            return UC_FALSE;
    }

    uc_memcpy(sNewBuf, pself->piLeftBuf, sRemainSz);
    uc_memcpy(sNewBuf + sRemainSz, buf, len);
    sNewBuf[sRemainSz + len] = '\0';

    ucfree(pself->piMainBuf); // memory notice
    pself->piMainBuf = sNewBuf;
    pself->piMainBufEnd = sNewBuf + sMallocSz; 
    return UC_TRUE;
}
/***************************************************************************
说明:   处理上次解析残余的buf和本次需要解析的buf，并确定本次可以解析的buf以及本次残余的buf
输入:	pself：WMLSaxParser指针
		buf：本次需要解析的buf
		len：本次需要解析的buf的长度
		final：是否是最后一段要解析的buf
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe 
DealParserBuf(WMLSaxParser *pself, UC_BYTE *buf, UC_INT len, UC_INT final)
{
    UC_INT sNeedsz, sRemainSz = pself->piLeftBufEnd - pself->piLeftBuf;

    sNeedsz = sRemainSz + len;
	// This is to void allocating memory every time
	// It only allocates the memory if the sNeedsz large than the memory that was allocated in last parsing.
	if (sNeedsz > pself->piMainBufEnd - pself->piMainBuf)
	{
        if(!CreateNewBuffer(pself,buf,len,sNeedsz,sRemainSz))
            return ERROR_NO_MEMORY;
        ResetBuffer(pself,pself->piMainBuf,sRemainSz,len);
	}
	else
	{
		if (pself->piLeftBufEnd + len > pself->piMainBufEnd) 
		{
			uc_memmove(pself->piMainBuf, pself->piLeftBuf, sRemainSz);
			uc_memcpy(pself->piMainBuf + sRemainSz, buf, len);
            pself->piMainBuf[sRemainSz + len] = '\0';
            
            ResetBuffer(pself,pself->piMainBuf,sRemainSz,len);
		}
		else if(pself->piLeftBufEnd)  
		{
			uc_memcpy(pself->piLeftBufEnd, buf, len);
            pself->piLeftBufEnd[len] = '\0';
			pself->piParseBuf = pself->piLeftBuf;
            
            ResetBuffer(pself,pself->piParseBuf,0,len);
		}
	}

	if (0 == pself->piParseBufEnd - pself->piParseBuf)
	{
		return ERROR_WAIT_FOR_DATA;
	}
	else
	{
		return ERROR_NONE;
	}
}

/***************************************************************************
说明:   收集一个标签如<a ...>后面跟的属性列表
输入:	pself：WMLSaxParser指针
		paPeeker：解析器游标的指针的指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
CollectAttriList( WMLSaxParser *pself, UC_BYTE **paPeeker)
{
	#define LEGAL_ATTRI_MAX_LEN					10240   // attribute do not allow to be greater than 10240

	WAPElement *psEle = &pself->piElement;
	WAPAttribute *psAttri = psEle->iAttriLst;

	UC_BYTE *psPeeker = *paPeeker;
	UC_BYTE *psParseBufEnd = pself->piParseBufEnd;
	UC_INT sDilimiter;

    do 
	{
		UC_BYTE *s;
		UC_INT sLen;

		ScanAttributeName(s);

		if (0 == (sLen = s - psPeeker))
		{
			break;
		}

		psAttri[psEle->iAttriNum].piAttri = NULL;
		psAttri[psEle->iAttriNum].piAttriValue = NULL;
		psAttri[psEle->iAttriNum].iAttriValueLen = 0;

		//psAttri[psEle->iAttriNum].piAttri = (Wap2Keyword*)findAttribute((const char *)psPeeker, sLen);
		psAttri[psEle->iAttriNum].piAttri = (Wap2Keyword*)getAttriInfo((const char *)psPeeker, sLen);


		psPeeker = s;

		SkipSlashSpace();

		if('=' == peek())
		{
			consume();
			SkipWhiteSpace();

			switch(sDilimiter = peek())
			{
			case '\'':
			case '\"':
				{
					consume();
					ScanAttributeValue(s);

					if (s <= psParseBufEnd)
					{
						psAttri[psEle->iAttriNum].piAttriValue = psPeeker;
                        
                        psAttri[psEle->iAttriNum].iAttriValueLen = processTransferCode(
                            psAttri[psEle->iAttriNum].piAttriValue,
                             s-psPeeker); 


						psPeeker = s + 1;
					}
					else
					{
						if (s - psPeeker > LEGAL_ATTRI_MAX_LEN)
						{
							pself->piLeftBufEnd = psPeeker + LEGAL_ATTRI_MAX_LEN;
						}
						
						return ERROR_WAIT_FOR_EXTRA_DATA;
					}
				}
				break;

			default:
				SenseAttributeValue(s);
				sLen = s - psPeeker;
				psAttri[psEle->iAttriNum].piAttriValue = psPeeker;
				psPeeker = s;
                psAttri[psEle->iAttriNum].iAttriValueLen = processTransferCode(
                    psAttri[psEle->iAttriNum].piAttriValue,
                    sLen); 
				break;
			}
		}

		if (psEle->iAttriNum < MAX_ATTRIBUTE_NUM -1
			&& psAttri[psEle->iAttriNum].piAttri)
		{
			psEle->iAttriNum ++;
		}

		SkipSlashSpace();

	} while (1);

	*paPeeker = psPeeker;

	return ERROR_NONE;
}

/***************************************************************************
说明:   解析script标签，目前处理方法是直接过滤掉
输入:	pself：WMLSaxParser指针
		paPeeker：解析器游标的指针的指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
ParseScript(WMLSaxParser *pself, UC_BYTE **paPeek)
{
	//跳过script
	#define SCRIPT_LENGTH     6	
    UC_BYTE *psPeeker = *paPeek;
    UC_BYTE* startOfPeeker = *paPeek;
    UC_BYTE* backforwardIndex = NULL;

    UC_BYTE *psParseBufEnd = pself->piParseBufEnd;
	
	while (psPeeker < psParseBufEnd)
	{
		while (psPeeker < psParseBufEnd && *psPeeker != '>')
		{
			psPeeker++;
		}
		backforwardIndex = psPeeker - SCRIPT_LENGTH; 
		if (backforwardIndex &&
            (0 == uc_strnicmp(backforwardIndex, (UC_BYTE *)"script", SCRIPT_LENGTH)))
		{
            while(backforwardIndex&&(*backforwardIndex-- == ' ')){}
            if (backforwardIndex&&(*backforwardIndex == '/'))
            {
               while(backforwardIndex&&(*backforwardIndex-- == ' ')){}
               if(backforwardIndex&&(*backforwardIndex == '<'))
               {
                   *paPeek = psPeeker + 1;
                   return ERROR_NONE;
               }
            }
		}
		consume();
	}
    //If script tag have attribute, this is incorrect,but the correct value will be set outside.
    *paPeek = startOfPeeker - SCRIPT_LENGTH -2 ;
    pself->piLeftBuf = *paPeek;
    pself->piLeftBufEnd = pself->piParseBufEnd + 1;
     
	return ERROR_WAIT_FOR_DELIMIT;
}


/***************************************************************************
说明:   解析结束标签
输入:	pself：WMLSaxParser指针
		paPeeker：解析器游标的指针的指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
ParseEndTag(WMLSaxParser *pself, UC_BYTE **paPeek)
{
 
	UC_INT sLen;
	UC_BYTE *psPeeker = *paPeek;
	const UC_BYTE *s;
	WAPElement *psEle = &pself->piElement;

	XMLErrorTYpe sError = ERROR_NONE;

	SkipSlashSpace();

	SenseElementName(s);

	if (0 == (sLen = s - psPeeker))
	{
		*paPeek = psPeeker + 1;
		return ERROR_NONE;
	}

	if (pself->iTextPos > 0)
	{
		//if (IsCharSpace(pself->piTextBuf[pself->iTextPos -1]))
		//{
		//	pself->iTextPos--;
		//}
        SendCharacter(pself,UC_FALSE);
	}

	//if (NULL != (psEle->piElement = (Wap2Keyword*)elementLook((const char *)psPeeker, sLen)))
	if (NULL != (psEle->piElement = getTagInfo((const char *)psPeeker, sLen)))
	{
		pself->piEndElementFunc(pself->piEventHander, psEle);
	}

	psPeeker = (UC_BYTE *)s;
	SkipToRightAngle();

	*paPeek = psPeeker + 1;

	return sError;
}

/***************************************************************************
说明:   解析开始标签
输入:	pself：WMLSaxParser指针
		paPeeker：解析器游标的指针的指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
ParseStartTag(WMLSaxParser *pself, UC_BYTE **paPeeker)
{
				 
	UC_BYTE *psPeeker = *paPeeker;
	const UC_BYTE *s;
	UC_INT sNmLen, sIsEmpty;
	WAPElement *psEle = &pself->piElement;
	XMLErrorTYpe sError = ERROR_NONE;

    
	SenseElementName(s);

	sNmLen = s - psPeeker;

	psEle->iAttriNum = 0;

	//psEle->piElement = (Wap2Keyword*)elementLook((const char *)psPeeker, sNmLen);
	psEle->piElement = getTagInfo((const char *)psPeeker, sNmLen);

	psPeeker = (UC_BYTE *)s;
 
	SkipSlashSpace();

	if ('>' != peek())
	{
		if (ERROR_WAIT_FOR_EXTRA_DATA == CollectAttriList(pself, &psPeeker))
		{
			pself->piLeftBuf = *paPeeker - 1;
			return ERROR_WAIT_FOR_EXTRA_DATA;
		}
	}

	sIsEmpty = ('/' == *(psPeeker - 1)); //Self-end <tag/>, e.g <BR/> <IMG/>

	if (pself->iTextPos > 0)
	{
		//if (IsCharSpace(pself->piTextBuf[pself->iTextPos -1]))
		//{
		//	pself->iTextPos--;
		//}
        SendCharacter(pself,UC_FALSE);
	}

	consume();

	if (psEle->piElement)
	{
		switch(psEle->piElement->token)
		{
		case WEB_TAG_TYPE_SCRIPT:
			sError = ParseScript(pself, &psPeeker);
			break;
		case WEB_TAG_TYPE_WML:
		case WEB_TAG_TYPE_CARD:
            // check wap version
            pself->bIsWap1x_m = UC_TRUE;

            // NOTE: jump through here, NO BREAK!!!
		default:
			pself->piStartElementFunc(pself->piEventHander, psEle, sIsEmpty);
			break;
		}
	}
    if (ERROR_WAIT_FOR_DELIMIT == sError)
    {
        UC_BYTE * sLeft = pself->piLeftBuf;
        UC_BYTE * sLeftEnd = pself->piLeftBufEnd;

        if (psPeeker != sLeft)
        {
            while (sLeft < sLeftEnd)
            {
                *psPeeker++ = *sLeft++;
            }
        }

        pself->piLeftBuf = *paPeeker - 1;
        pself->piLeftBufEnd = psPeeker + (sLeftEnd - sLeft);

        return ERROR_WAIT_FOR_EXTRA_DATA;
    }

	*paPeeker = psPeeker;

	return sError;
}


/***************************************************************************
说明:   获取实体对应的值
输入:	aSrc：实体的源
输出:   aValue：实体的值
返回:	实体在源中的长度
作者:   danwl
记录:	N/A
****************************************************************************/
UC_INT
WMLGetEntityValue(const UC_BYTE *aSrc, UC_UINT *aValue)
{

	UC_UINT sRet = 0;
	const UC_BYTE *s;
	const Entity *paTable = S_EntityTable;

	if (!aSrc || !aValue)
	{
		return 0;
	}
	s = aSrc + 1;

	if ('#' == *s)
	{
		s++;

		if ('x' == *s || 'X' == *s)
		{
			s++;
			while (IsCharHex(*s))
			{
				sRet <<= 4;

				if (IsCharDecimal(*s))
				{
					sRet += (*s++ - '0');
				}
				else
				{
					sRet += (upper2lower(*s++) - 'a' + 10);
				}
			}
		}
		else
		{
			while (IsCharDecimal(*s))
			{
				sRet = (sRet << 3) + (sRet << 1);
				sRet += (*s++ - '0');
			}
		}

		*aValue = sRet;
		return s +1 - aSrc;  //Need to include the ;
	}
	else
	{
		UC_INT i;
		for (i = S_EntityTableSZ - 1; i >= 0; i--)
		{
			if (!uc_strnicmp((UC_BYTE*)paTable[i].piEntity, (UC_BYTE*)s, paTable[i].iEntiLen))
			{
				*aValue = paTable[i].iValue;
				return paTable[i].iEntiLen + 1;
			}
		}

		*aValue = 0;
		return 0;
	}
}

static void sendWap10Variable(WMLSaxParser *pself)
{
	WAPElement *psEle;
    Wap2Keyword eleToken;
    Wap2Keyword attrToken;
    WAPAttribute *psAttri;

    if ((NULL == pself->piTextBuf) || (0 >= pself->iTextPos))
    {
        return;
    }

    // dummy element
	psEle = &pself->piElement;
    eleToken.name   = NULL;
    eleToken.token  = WEB_TAG_TYPE_WAP1X_VAR;

    psEle->piElement = &eleToken;

    // dummy attribute
    psAttri       = psEle->iAttriLst;
    attrToken.name  = NULL;
    attrToken.token = WEB_ATTRIBUTE_TYPE_NAME;
    psAttri[0].piAttri          = &attrToken;
    psAttri[0].piAttriValue     = pself->piTextBuf;
    psAttri[0].iAttriValueLen   = pself->iTextPos;
    psEle->iAttriNum = 1;

    pself->piStartElementFunc(pself->piEventHander, psEle, UC_TRUE);
}

/***************************************************************************
说明:   解析文本
输入:	pself：WMLSaxParser指针
		paPeeker：解析器游标的指针的指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
ParseText(WMLSaxParser *pself, UC_BYTE **paPeeker)
{
	UC_BYTE *psPeeker = *paPeeker;
	const UC_BYTE *psParseBufEnd = pself->piParseBufEnd;
	const UC_BYTE *s;

	do
	{
        UC_BYTE bHasSpace = UC_FALSE;
        for ((s) = psPeeker; IsCharSpace(*psPeeker); consume())
        {
            bHasSpace = UC_TRUE;
        }
		
        if (bHasSpace)
		{
            if (pself->iDollarFlag == VARIABLE_STARTED)
            {
                pself->iDollarFlag = VARIABLE_ENCLOSED;

                //notify internal variable element
                sendWap10Variable(pself);

                pself->iTextPos = 0;
            }

            PushText(pself, ' ');
            pself->bFilterTailSpace_m = UC_TRUE;
		}
        else
        {
            pself->bFilterTailSpace_m = UC_FALSE;
        }

		for (s = psPeeker; !IsCharTextEnd(peek()); consume())
		{
            if (VARIABLE_STARTED == pself->iDollarFlag)
            {
                if (peek() == ')' || peek() == ' ')
                {
                    pself->iDollarFlag = VARIABLE_ENCLOSED;

                    //notify internal variable element
                    sendWap10Variable(pself);

                    pself->iTextPos = 0;
                }
                else
                {
                    PushText(pself, peek());
                }
            }
            else
            {
                PushText(pself, peek());
            }
		}

		switch (peek())
		{
		case '<':
			*paPeeker = psPeeker;
            if (pself->iDollarFlag == VARIABLE_STARTED)
            {
                pself->iDollarFlag = VARIABLE_ENCLOSED;

                //notify internal variable element
                sendWap10Variable(pself);

                pself->iTextPos = 0;
            }
			return ERROR_NONE;

		case '&':
			{
				UC_INT sLen = 0;
				UC_UINT sValue = 0;
				sLen = WMLGetEntityValue(psPeeker, &sValue);

				if (sValue)
				{
					if (sValue < 0x80)
					{
						PushText(pself, (UC_BYTE)sValue);
					}
					else
					{
						UC_INT i, ct;
						UC_BYTE text[4];
						
						ct = unicode_to_utf8(text, (UC_WORD)sValue);
						for (i = 0; i < ct; i++)
						{
							PushText(pself, text[i]);
						}
					}

					consumen(sLen);
					if (';' == peek())
					{
						consume();
					}
				}
				else
				{
					PushText(pself, *psPeeker++);
				}

                // non-space character, needn't check tail space
                pself->bFilterTailSpace_m = UC_FALSE;
			}
			break;

		case '$':
            {
                UC_BYTE cNext;
                // if it's wap2.0 page, don't take variable into account
                if (!pself->bIsWap1x_m)
                {
					PushText(pself, *psPeeker++);

                    pself->bFilterTailSpace_m = UC_FALSE;
                    break;
                }

                consume();

                cNext = peek();
                switch(cNext)
                {
                    case ' ':
                        break;
                    case '$':
                        consume();
                        PushText(pself, '$');
                        break;
                    case '(':           // $(xxx)
                        // skip '('
                        consume();
                        // Skip, need not break here
                    default:
                        if (pself->iTextPos > 0)
                        {
                            // keep space among text
                            pself->bFilterTailSpace_m = UC_FALSE;

                            SendCharacter(pself,UC_FALSE);
                        }

                        pself->iDollarFlag = VARIABLE_STARTED;
                        break;
                }
            }
			break;
		default:

			if ('>' == peek())
			{
				if (psPeeker >= psParseBufEnd)
				{
					if (psPeeker == psParseBufEnd)
					{
						PushText(pself, '>');
					}

					*paPeeker = psPeeker;
					return ERROR_WAIT_FOR_EXTRA_DATA;
				}

				PushText(pself, *psPeeker++);
			}

			break;
		}

	}while (1);
}

/***************************************************************************
说明:   解析Legacy
输入:	pself：WMLSaxParser指针
		paPeeker：解析器游标的指针的指针
输出:   N/A
返回:	
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
parseLegacy(WMLSaxParser *pself, UC_BYTE **paPeeker)
{


	#define CDATA_LEN			8 //![CDATA[
	#define COMMENT_LEN			3 //!--

	UC_BYTE *s = *paPeeker;
	const UC_BYTE *psParseBufEnd = pself->piParseBufEnd;
 

	if ('[' == *(s + 1)
		&& 'c' == upper2lower(*(s + 2))
		&& 'd' == upper2lower(*(s + 3))
		&& 'a' == upper2lower(*(s + 4))
		&& 't' == upper2lower(*(s + 5))
		&& 'a' == upper2lower(*(s + 6))
		&& '[' == upper2lower(*(s + 7)))
	{
		s += CDATA_LEN;

		do 
		{
			if (']' == *(s) 
				&& ']' == *(s + 1)
				&& '>' == *(s + 2))
			{
				break;
			}

			PushText(pself, *s);
			
		} while (++s <= psParseBufEnd);

		if (s > psParseBufEnd)
		{
			UC_BYTE * sLeft = pself->piLeftBuf;
			UC_BYTE * sLeftEnd = pself->piLeftBufEnd;

			pself->piLeftBuf = *paPeeker - 1;

			s = pself->piLeftBuf + CDATA_LEN + 1;

			if (s != sLeft)
			{
				while (sLeft < sLeftEnd)
				{
					*s++ = *sLeft++;
				}
			}

			pself->piLeftBufEnd = s + (sLeftEnd - sLeft);;
			
			return ERROR_WAIT_FOR_EXTRA_DATA;
		}
		else
		{
			*paPeeker = s + 3;
			return ERROR_NONE;
		}
	}
	else if ('-' == *(s + 1) && '-' == *(s + 2))
	{
		s += COMMENT_LEN;

		do 
		{
			if ('-' == *(s) 
				&& '-' == *(s + 1)
				&& '>' == *(s + 2))
			{
				break;
			}

		} while (++s <= psParseBufEnd);

		if (s > psParseBufEnd)
		{
			pself->piLeftBuf = *paPeeker - 1;
			pself->piLeftBufEnd = *paPeeker + COMMENT_LEN;
             
			return ERROR_WAIT_FOR_EXTRA_DATA;
		}
		else
		{
			*paPeeker = s + 3;
			return ERROR_NONE;
		}
	}
	else
	{
		do
		{
			s++;
		}while ('>' != *s);

		*paPeeker = s + 1;
		return ERROR_NONE;
	}
}


/***************************************************************************
说明:   sax解析主架构
输入:	s1：串1
		s2：串2
		n: 需要比较的长度
输出:   N/A
返回:	N/A
作者:   danwl
记录:	N/A
****************************************************************************/
static XMLErrorTYpe
DoKernelParse(WMLSaxParser *pself)
{
	XMLErrorTYpe sError = ERROR_NONE; 

	UC_BYTE *psPeeker = pself->piParseBuf;
	UC_BYTE *psParseBufEnd = pself->piParseBufEnd;
 
	do 
	{
		SkipWhiteSpace();
        
		switch(peek())
		{
		case '<':
			consume();

			switch (peek())
			{
			case '/':
				sError = ParseEndTag(pself, &psPeeker);
				break;

			case '?':
			case '!':
				sError = parseLegacy(pself, &psPeeker);
				break;

			default:
				if (IsCharFirstName(peek()))
				{
					sError = ParseStartTag(pself, &psPeeker);
				}
				else
				{
					PushText(pself, '<');
					sError = ParseText(pself, &psPeeker);
				}

				break;
			}

			break;

		default:
			sError = ParseText(pself, &psPeeker);
			break;
		}

		switch (sError)
		{
		case ERROR_WAIT_FOR_EXTRA_DATA:
			return ERROR_WAIT_FOR_EXTRA_DATA;
			
		default:
			break;
		}
  

	} while (psPeeker <= psParseBufEnd);

	return ERROR_WAIT_FOR_DATA;
}





/***************************************************************************
说明:   解析一段内存，该段内存是一个完整的页面文件中的一段。
输入:	pself：WMLSaxParser对象指针的指针
		buf：需要解析的那段内存
		len：需要解析的内存的长度
		isFinal：该段内存是不是一个完整页面的最后一部分。
输出:   N/A
返回:	解析错误类型
作者:   danwl
记录:	N/A
****************************************************************************/
XMLErrorTYpe 
WmlSaxParserParseBuf(WMLSaxParser *pself, UC_BYTE *buf, UC_INT len, UC_INT isFinal)
{
	XMLErrorTYpe sError;
	
	switch (pself->iParsingStatus)
	{
	case PARSING_SUSPENDED:
		return ERROR_SUSPENDED;
		
	case PARSING_FINISHED:
		return ERROR_FINISHED;
	}
	
	sError = DealParserBuf(pself, buf, len, isFinal);
	switch (sError)
	{
	case ERROR_NONE:
		sError = DoKernelParse(pself);
		break;
		
	case ERROR_NO_MEMORY:
		
		//降低解析需要的内存
		pself->iParsingStatus = PARSING_FINISHED;
		return ERROR_FINISHED;
		
	case ERROR_WAIT_FOR_DATA:
		
		if (isFinal || pself->piLeftBufEnd - pself->piLeftBuf >= PARSE_BUF_UNIT_SIZE)
		{
			*(pself->piLeftBufEnd) = '>';
			pself->piLeftBuf = pself->piLeftBufEnd;
			pself->piParseBufEnd = pself->piLeftBufEnd - 1;
			sError = DoKernelParse(pself);
		}
		
		break;
		
	default:
		break;
	}
	
	if (isFinal && pself->iTextPos > 0)
	{
        SendCharacter(pself,UC_FALSE);
	}
	
	return sError;
}

/***************************************************************************
说明:   销毁WMLSaxParser对象
输入:	psParse 	WMLSaxParser对象指针的指针
输出:   N/A
返回:	N/A
作者:   danwl
记录:	N/A
****************************************************************************/
void WmlSaxParserDelete(WMLSaxParser **psParse)
{
	WMLSaxParser *pself = *psParse;
	
	if (!pself)
	{
		return;
	}
	
	ucfree(pself->piMainBuf);
	ucfree(pself->piTextBuf);
	
	ucfree(pself);
	*psParse = NULL;
}

/***************************************************************************
说明:   创建WMLSaxParser对象
输入:	N/A	
输出:   N/A
返回:	WMLSaxParser指针
作者:   danwl
记录:	N/A
****************************************************************************/
WMLSaxParser * WmlSaxParserNew(void)
{
	WMLSaxParser *psParser;
	psParser = (WMLSaxParser *)ucmalloc(sizeof(WMLSaxParser));

	if (!psParser)
	{
		return NULL;
	}

	uc_memset(psParser, 0, sizeof(WMLSaxParser));

	if (NULL == (psParser->piTextBuf = (UC_BYTE *)ucmalloc(INITIAL_BUF_SIZE + MALLOC_EXTRA_SIZE)))
	{
		WmlSaxParserDelete(&psParser);
		return NULL;
	}

    // init flag
    psParser->bIsWap1x_m        = UC_FALSE;
    psParser->bFilterTailSpace_m= UC_FALSE;

	psParser->iTextBufLen = INITIAL_BUF_SIZE;
	
	return psParser;
}

/***************************************************************************
说明:   为sax解析器设置处理事件流的对象和处理函数
输入:	paParser：sax解析器对象指针
		paEventHander：处理sax事件流的对象
		psStartTagHand：开始标签处理函数
		psEndTagHand：结束标签处理函数
		psCharacterHand：文本处理函数
		psStyleHand：样式处理函数
输出:   N/A
返回:	替换后的字符串
作者:   danwl
记录:	N/A
****************************************************************************/
void WmlSaxParserSetEventHandler(WMLSaxParser * paParser,
			void * paEventHander, 
			WMLStartElementHandlerFunc psStartTagHand,
			WMLEndElementHandlerFunc psEndTagHand,
			WMLCharacterDataHandlerFunc psCharacterHand)
{
	WMLSaxParser *psParser = (WMLSaxParser *)paParser;

	if (!psParser)
	{
		return;
	}

	psParser->piEventHander = paEventHander;

	psParser->piStartElementFunc = psStartTagHand;
	psParser->piEndElementFunc = psEndTagHand;
	psParser->piCharacterFunc = psCharacterHand;

}


UC_INT processTransferCode(UC_BYTE* paSrc,UC_INT len)
{
    UC_INT sRet;
    UC_INT sLen = len;
    UC_UINT sValue;
    UC_BYTE *psSrcEnd = paSrc + sLen;
    UC_BYTE *psHead = paSrc;
    UC_INT totalCount = 0;
    UC_INT moveCount = len;
    UC_INT ct;

    while (paSrc < psSrcEnd)
    {
        if ('&' == *paSrc)
        {
            sValue = 0;
            sRet = WMLGetEntityValue((UC_BYTE *)paSrc, &sValue);
            if (sValue)
            {
                if (sValue < 0x80)
                {
                    ct = 1;                   
                    *paSrc++ = (UC_BYTE)sValue;
                }
                else
                {
                    UC_INT i;
                    UC_BYTE text[4];

                    ct = unicode_to_utf8(text, (UC_WORD)sValue);
                    for (i = 0; i < ct; i++)
                    {
                        *paSrc++ = text[i];
                    } 
                }

                moveCount = len - sRet;
                uc_memmove(paSrc,paSrc+sRet -ct,moveCount);
                *(paSrc+moveCount) = 0;
                psSrcEnd = paSrc+moveCount;
                totalCount = totalCount + sRet - ct;
                len = moveCount;
            }
            else
            {
                paSrc++;
                len--;
            }
        }
        else
        {
            paSrc++;
            len--;
        }
    }
    paSrc = psHead; //Recover to start of the ptr
    return (sLen- totalCount);
}

