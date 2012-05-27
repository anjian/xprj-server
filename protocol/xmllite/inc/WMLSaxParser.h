/*******************************************************************************
* Copyright (C) 2004 - 2010 UCWEB Technology Ltd.
* All rights reserved.
* 文件名:           WMLSaxParser.h  
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
*******************************************************************************/

#ifndef _SAX_WML_PARSER_H_
#define _SAX_WML_PARSER_H_

#include <interface/protocol/UcXmlParserCallBack.h>


/*
 * =============================================================================
 *  宏定义区
 * =============================================================================
 */




//解析器能给缓存的最大文本个数，如果一段连续的文本长度超过了该值，解析器就会向外发出文本事件		
#define TEXT_BUF_MAX_LEN					(3*1024 + 256)
   
// DATA_POOL_CHUNK_SIZE is define here is because removed the WMLDataPool.h
#define	DATA_POOL_CHUNK_SIZE					(1024 + 256) 
#define PARSE_BUF_UNIT_SIZE					DATA_POOL_CHUNK_SIZE

//malloc时候的额外空间，用于存放‘/0’
#define MALLOC_EXTRA_SIZE					2   // Sam: one char should be enough?




/*
 * =============================================================================
 *  全局函数声明区区
 * =============================================================================
 */

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



typedef enum
{
    PARSING_INITIALIZED,
    PARSING_PARSING,
    PARSING_SUSPENDED,
    PARSING_FINISHED

}XMLParsingStatus;

typedef enum
{
    ERROR_START,

    ERROR_FINISHED,
    ERROR_SUSPENDED,

    ERROR_NO_MEMORY,
    ERROR_WAIT_FOR_DATA,
    ERROR_WAIT_FOR_DELIMIT,
    ERROR_WAIT_FOR_EXTRA_DATA,
    ERROR_NON_PTR,
    WAITING_FOR_CSS,

    ERROR_NONE

}XMLErrorTYpe;


typedef struct  
{
    UC_WORD iValue;			//实体对应的值
    UC_WORD iEntiLen;		//实体的长度
    UC_CHAR *piEntity;		//实体的名称，如nbsp,amp
}Entity;

typedef enum {
    NO_DOLLAR_OCCURRED = 0,
    VARIABLE_STARTED,                  
    VARIABLE_ENCLOSED
}WapVariableFlag;


struct tagWAPParser {

    UC_BYTE *piParseBuf;			//本次可解析的buf首
    UC_BYTE *piParseBufEnd;			//本次可解析的buf尾

    UC_BYTE *piLeftBuf;				//本次剩余的不可解析的buf首
    UC_BYTE *piLeftBufEnd;			//本次剩余的不可解析的buf尾

    UC_BYTE *piMainBuf;				//解析器上下文buf，用于承载本次要解析的原始数据以及上一次解析的残余数据
    UC_BYTE *piMainBufEnd;			//解析器上下文buf的结束

    WAPElement piElement;			//解析器生成的临时的元素

    UC_BYTE *piTextBuf;				//文本缓冲buf
    UC_UINT iTextPos;				//本次文本的结束位置
    UC_UINT iTextBufLen;			//文本缓冲buf的最大长度
    WapVariableFlag iDollarFlag;			//文本中是否含有$符号   

    UC_BOOL bIsWap1x_m;             // is wap 1.x page
    UC_BOOL bFilterTailSpace_m;     // Does SendCharacter need check tailer space

    XMLParsingStatus iParsingStatus;//解析器的状态

    void * piEventHander;			//处理sax事件流的对象

    WMLStartElementHandlerFunc piStartElementFunc;	//开始标签处理函数
    WMLEndElementHandlerFunc piEndElementFunc;		//结束标签处理函数
    WMLCharacterDataHandlerFunc piCharacterFunc;	//文本处理函数
};

typedef struct tagWAPParser WMLSaxParser;

/*
 * =============================================================================
 *  全局函数声明区区
 * =============================================================================
 */
/***************************************************************************
说明:   高效的忽略大小写的指定比较长度的字符串比较函数
输入:	s1：串1
		s2：串2
		n: 需要比较的长度
输出:   N/A
返回:	N/A
作者:   danwl
记录:	N/A
****************************************************************************/


/***************************************************************************
说明:   创建WMLSaxParser对象
输入:	N/A	
输出:   N/A
返回:	WMLSaxParser指针
作者:   danwl
记录:	N/A
****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

WMLSaxParser * 
WmlSaxParserNew(void);

/***************************************************************************
说明:   销毁WMLSaxParser对象
输入:	psParse 	WMLSaxParser对象指针的指针
输出:   N/A
返回:	N/A
作者:   danwl
记录:	N/A
****************************************************************************/
void 
WmlSaxParserDelete(WMLSaxParser **paParse);

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
WmlSaxParserParseBuf(WMLSaxParser *paParse, UC_BYTE *buf, UC_INT len, UC_INT isFinal);

void WmlSaxParserSetEventHandler(WMLSaxParser * paParser,
                                 void * paEventHander, 
                                 WMLStartElementHandlerFunc psStartTagHand,
                                 WMLEndElementHandlerFunc psEndTagHand,
                                 WMLCharacterDataHandlerFunc psCharacterHand);




#ifdef __cplusplus
}
#endif
/***************************************************************************
说明:   获取实体对应的值
输入:	aSrc：实体的源
输出:   aValue：实体的值
返回:	实体在源中的长度
作者:   danwl
记录:	N/A
****************************************************************************/
UC_INT
WMLGetEntityValue(const UC_BYTE *aSrc, UC_UINT *aValue);





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
UC_INT unicode_to_utf8(UC_BYTE* utf8, UC_WORD ucs2);

UC_INT processTransferCode(UC_BYTE* paSrc,UC_INT len);


#endif
