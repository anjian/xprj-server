////////////////////////////////////////////////////////////////////////////////
// File Name:      DataOutputStream.h                                 
// Author:                                                                  
// Date:           2012-3-16                                         
// Copyright:      2012, JW, All Rights Reserved.                            
// Description:    The abstract class for the webdata output.  
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2012/03/16 |  J. Wang    |  Initial creation.                                  
// -------------------------------------------------------------------------
//                                                                            
////////////////////////////////////////////////////////////////////////////////
#ifndef __DATA_OUTPUT_STREAM_H__
#define __DATA_OUTPUT_STREAM_H__

enum OutputStreamRetCode_e
{
    OUTPUT_GEN_COMPLETE,
    OUTPUT_GEN_CONTINUE,
    OUTPUT_GEN_BUF_NOT_ENOUGH,
    OUTPUT_GEN_ERROR
};

class DataOutputStream_i
{
    public:
        virtual ~DataOutputStream_i()   {}

        virtual bool init()             { return true;}

        virtual int getContentLength() = 0; 
        virtual OutputStreamRetCode_e generate(char* sBuf, const int nBufLen, int& nContentLen) = 0;
        virtual void reset() = 0;

        bool appendByte (char* sBuf, const int nBufLen, int& nContentLen, char cContent);
        bool appendInt16(char* sBuf, const int nBufLen, int& nContentLen, int nNum);
        bool appendInt32(char* sBuf, const int nBufLen, int& nContentLen, int nNum);
        bool appendInt64(char* sBuf, const int nBufLen, int& nContentLen, long long nNum);
        bool appendString(char* sBuf, const int nBufLen, int& nContentLen, char* sStr, int nLen);
};

#endif
