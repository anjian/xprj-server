////////////////////////////////////////////////////////////////////////////////
// File Name:      os_type.h                                          
// Author:                                                                  
// Date:           2012-02-13                                               
// Copyright:      2012 J.W, All Rights Reserved.                            
// Description:    OS related marco and type definition                     
//                                                                          
//**************************************************************************
//                         History                                          
// -------------------------------------------------------------------------
//    DATE    |    NAME     |  DESCRIPTION                                  
// -------------------------------------------------------------------------
// 2012/02/13 | J. Wang     | Create								        
// -------------------------------------------------------------------------
//																		    
////////////////////////////////////////////////////////////////////////////////

#ifndef _OS_TYPE_H_
#define _OS_TYPE_H_

#include <stddef.h>

//////////////////////////////////////////////////////////////////////////////////////////
// To enable your code to work on both 32-bit and 64-bit systems, note the following
// regarding declarations:
// refer to: http://www.ibm.com/developerworks/library/l-port64/index.html
//
//  .) Declare integer constants using "L" or "U", as appropriate.
//
//  .) Ensure that an unsigned int is used where appropriate to prevent sign extension.
//
//  .) If you have specific variables that need to be 32-bits on both platforms, define
//  the type to be int.
//
//  .) If the variable should be 32-bits on 32-bit systems and 64-bits on 64-bit systems,
//  define them to be long.
//
//  .) Declare numeric variables as int or long for alignment and performance. Don't try
//  to save bytes using char or short.
//
//  .) Declare character pointers and character bytes as unsigned to avoid sign extension
//  problems with 8-bit characters.
//////////////////////////////////////////////////////////////////////////////////////////

//typedef int 					INT;

#if 0
typedef	unsigned char 			BYTE;
typedef	unsigned short			WORD;
typedef	unsigned int			DWORD;
typedef unsigned long			LONG;
typedef	unsigned int			UINT;

typedef	signed char				INT8;
typedef	signed short			INT16;
typedef	signed int				INT32;

typedef unsigned char			UINT8;
typedef unsigned short			UINT16;
typedef unsigned int 			UINT32;
typedef double                  UC_REAL;

#ifdef _MSC_VER
typedef __int64                 INT64;
typedef unsigned __int64        UINT64;
#elif defined __GNUC__
typedef long long               INT64;
typedef unsigned long long      UINT64;
#elif defined __arm
typedef __int64                 INT64;
typedef unsigned __int64        UINT64;
#endif

typedef char                    CHAR;
typedef unsigned char           UCHAR;
typedef UINT16                  WCHAR;
typedef WCHAR*                  PWSTRING;		
typedef char*                   PSTRING;

typedef BYTE                    BOOL;

#define	TRUE 			        1
#define	FALSE			        0

#define ATTRIBUTE_NOT_FOUND     -1
#endif

typedef void (*x_object_notify)(void * aFrom,void * aData);

////////////////////////////////////////////////////////////////////////////
//begins functions
#define SAFE_DELETE_PTR(ptr) if (NULL != ptr)\
                             {               \
                               delete ptr;   \
                               ptr = NULL;   \
                             }

#define SAFE_DELETE_ARRAY(ptr) if (NULL != ptr)\
                             {               \
                               delete []ptr;   \
                               ptr = NULL;   \
                             }
////////////////////////////////////////////////////////////////////////////////

#ifndef NULL
#define NULL 0
#endif

#ifdef _MSC_VER 
#define _RESTRICT_QUANLIFIER __restrict
#elif defined __GNUC__
#define _RESTRICT_QUANLIFIER __restrict
#else
#define _RESTRICT_QUANLIFIER 
#endif

#define    ONE_SECOND       1000  //1000 ms
#define    ONE_MINUTE       60000 //60's

#endif //__OS_TYPE_H_
