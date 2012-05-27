////////////////////////////////////////////////////////////////////////////////
// File Name:      TempObject.h
// Author:                                                                  
// Date:           2012-02-14
// Copyright:      2012 J.W, All Rights Reserved.                            
// Description:    temp object base class
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2012/02/14 | J. Wang     | Create								        
// -------------------------------------------------------------------------
//																		    
////////////////////////////////////////////////////////////////////////////////
//
#ifndef __TEMP_OBJECT_H_
#define __TEMP_OBJECT_H_

#include <interface/baseos/TransientMemory.h>

class TempObject_i
{
    public:
        virtual ~TempObject_i() {}

        void* operator new (size_t size)   // overload to allocate from transient pool
        {
            return TR_MEMORY_ALLOC(size);
        }

        void operator delete (void *p)     // cooresponding to new
        {
            TR_MEMORY_FREE(p);
        }
};


#endif // _UC_TEMP_OBJECT_H_
