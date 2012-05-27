/*******************************************************************************
* Copyright (C) 2004 - 2010 UCWEB Technology Ltd.
* All rights reserved.
* �ļ���:           WMLSaxParser.h  
* ����:             danwl
* �汾:             $Revision$
* ��������:         2010-07-16
* ������Ŀ��		UcWeb�ͻ���
* ģ������:         ��������
* �汾��Ϣ:         ����
* ƽ̨��			MTK
* ��Ҫ�������书��:
*					
* ��¼:
// yyyymmmdd Changed by      Change Reason
// --------- --------------- --------------------------------------------------
// 2010Dec02 Yangxm          Change for Brew7.2 
*******************************************************************************/

#ifndef _SAX_WML_PARSER_H_
#define _SAX_WML_PARSER_H_

#include <interface/protocol/UcXmlParserCallBack.h>


/*
 * =============================================================================
 *  �궨����
 * =============================================================================
 */




//�������ܸ����������ı����������һ���������ı����ȳ����˸�ֵ���������ͻ����ⷢ���ı��¼�		
#define TEXT_BUF_MAX_LEN					(3*1024 + 256)
   
// DATA_POOL_CHUNK_SIZE is define here is because removed the WMLDataPool.h
#define	DATA_POOL_CHUNK_SIZE					(1024 + 256) 
#define PARSE_BUF_UNIT_SIZE					DATA_POOL_CHUNK_SIZE

//mallocʱ��Ķ���ռ䣬���ڴ�š�/0��
#define MALLOC_EXTRA_SIZE					2   // Sam: one char should be enough?




/*
 * =============================================================================
 *  ȫ�ֺ�����������
 * =============================================================================
 */

/***************************************************************************
˵��:   Ϊsax���������ô����¼����Ķ���ʹ�����
����:	paParser��sax����������ָ��
		paEventHander������sax�¼����Ķ���
		psStartTagHand����ʼ��ǩ������
		psEndTagHand��������ǩ������
		psCharacterHand���ı�������
		psStyleHand����ʽ������
���:   N/A
����:	�滻����ַ���
����:   danwl
��¼:	N/A
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
    UC_WORD iValue;			//ʵ���Ӧ��ֵ
    UC_WORD iEntiLen;		//ʵ��ĳ���
    UC_CHAR *piEntity;		//ʵ������ƣ���nbsp,amp
}Entity;

typedef enum {
    NO_DOLLAR_OCCURRED = 0,
    VARIABLE_STARTED,                  
    VARIABLE_ENCLOSED
}WapVariableFlag;


struct tagWAPParser {

    UC_BYTE *piParseBuf;			//���οɽ�����buf��
    UC_BYTE *piParseBufEnd;			//���οɽ�����bufβ

    UC_BYTE *piLeftBuf;				//����ʣ��Ĳ��ɽ�����buf��
    UC_BYTE *piLeftBufEnd;			//����ʣ��Ĳ��ɽ�����bufβ

    UC_BYTE *piMainBuf;				//������������buf�����ڳ��ر���Ҫ������ԭʼ�����Լ���һ�ν����Ĳ�������
    UC_BYTE *piMainBufEnd;			//������������buf�Ľ���

    WAPElement piElement;			//���������ɵ���ʱ��Ԫ��

    UC_BYTE *piTextBuf;				//�ı�����buf
    UC_UINT iTextPos;				//�����ı��Ľ���λ��
    UC_UINT iTextBufLen;			//�ı�����buf����󳤶�
    WapVariableFlag iDollarFlag;			//�ı����Ƿ���$����   

    UC_BOOL bIsWap1x_m;             // is wap 1.x page
    UC_BOOL bFilterTailSpace_m;     // Does SendCharacter need check tailer space

    XMLParsingStatus iParsingStatus;//��������״̬

    void * piEventHander;			//����sax�¼����Ķ���

    WMLStartElementHandlerFunc piStartElementFunc;	//��ʼ��ǩ������
    WMLEndElementHandlerFunc piEndElementFunc;		//������ǩ������
    WMLCharacterDataHandlerFunc piCharacterFunc;	//�ı�������
};

typedef struct tagWAPParser WMLSaxParser;

/*
 * =============================================================================
 *  ȫ�ֺ�����������
 * =============================================================================
 */
/***************************************************************************
˵��:   ��Ч�ĺ��Դ�Сд��ָ���Ƚϳ��ȵ��ַ����ȽϺ���
����:	s1����1
		s2����2
		n: ��Ҫ�Ƚϵĳ���
���:   N/A
����:	N/A
����:   danwl
��¼:	N/A
****************************************************************************/


/***************************************************************************
˵��:   ����WMLSaxParser����
����:	N/A	
���:   N/A
����:	WMLSaxParserָ��
����:   danwl
��¼:	N/A
****************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

WMLSaxParser * 
WmlSaxParserNew(void);

/***************************************************************************
˵��:   ����WMLSaxParser����
����:	psParse 	WMLSaxParser����ָ���ָ��
���:   N/A
����:	N/A
����:   danwl
��¼:	N/A
****************************************************************************/
void 
WmlSaxParserDelete(WMLSaxParser **paParse);

/***************************************************************************
˵��:   ����һ���ڴ棬�ö��ڴ���һ��������ҳ���ļ��е�һ�Ρ�
����:	pself��WMLSaxParser����ָ���ָ��
		buf����Ҫ�������Ƕ��ڴ�
		len����Ҫ�������ڴ�ĳ���
		isFinal���ö��ڴ��ǲ���һ������ҳ������һ���֡�
���:   N/A
����:	������������
����:   danwl
��¼:	N/A
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
˵��:   ��ȡʵ���Ӧ��ֵ
����:	aSrc��ʵ���Դ
���:   aValue��ʵ���ֵ
����:	ʵ����Դ�еĳ���
����:   danwl
��¼:	N/A
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
