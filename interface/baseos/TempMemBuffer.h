////////////////////////////////////////////////////////////////////////////////
// File Name:      TempMemBuffer.h
// Author:                                                                  
// Date:           2012-03-01
// Copyright:      2011 UC, All Rights Reserved.                            
// Description:    Auto extendable memory buffer, and allocated from transient
//                 memory pool
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2012/03/01 | J. Wang     | Create								        
// -------------------------------------------------------------------------
//																		    
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMP_MEM_BUFFER_H_
#define __TEMP_MEM_BUFFER_H_

#include <interface/baseos/TempObject.h>
//#include <interface/memory.h>

//#include <common/inc/MyStdLibrary.h>

class TempMemBuffer_c : public TempObject_i
{
    public:
        TempMemBuffer_c();
        ~TempMemBuffer_c();

        inline char* getBuffer()            { return pBuf_m; }
        inline int getLength()              { return nContentLen_m; }
        inline void clear()                 { nContentLen_m = 0; }

        bool append(char cChar);
        bool append(const char* pContent, int nLen);

        bool cutAsString();

    protected:
        bool extendBuffer(int nLen);

    private:
        char* pBuf_m;
        int nBufLen_m;
        int nContentLen_m;
};

#endif
