#ifndef __BSEARCH_H_
#define __BSEARCH_H_

#include <common/inc/MyStdLibrary.h>

////////////////////////////////////////////////////////////////////////////////
// Compares two string ignore case
// p1 is none-zero end string.
// p2 is zero end string.
// Return value: as strcmp()
////////////////////////////////////////////////////////////////////////////////
int compareIc(const char *p1,int nP1Len,const char *p2);

////////////////////////////////////////////////////////////////////////////////
// b-search template function:
//
// .) input struct/class must have a primary key field named as s_name
// .) return the index of array if found, else return -1
////////////////////////////////////////////////////////////////////////////////

template<class T> int b_search(const T table[], const int nTableSize, const char* sName,int nLen = -1)
{
    if (-1 == nLen)
    {
        nLen = xstrlen(sName);
    }
    int nLow  = 0;
    int nHigh = nTableSize - 1;    // Last tag index 
    int nMid;
    int nCond;

    // Binary search 
    while (nLow <= nHigh)
    {
        nMid = ((nLow + nHigh) >> 1);
        if ((nCond = compareIc(sName, nLen, table[nMid].s_name)) < 0)
        {
            nHigh = nMid - 1;
        }
        else if (nCond > 0)
        {
            nLow = nMid + 1;
        }
        else
        {
            return nMid;
        }
    }

    return -1;
}



#endif
