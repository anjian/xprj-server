
#include <common/inc/bsearch.h>
#include <common/inc/MyCType.h>

////////////////////////////////////////////////////////////////////////////////
// Compares two string ignore case
// p1 is none-zero end string.
// p2 is zero end string.
// Return value: as strcmp()
////////////////////////////////////////////////////////////////////////////////
int compareIc(const char *p1, int nP1Len, const char *p2)
{
    int i = 0;
    for (i = 0; ((i < nP1Len)&& *p2) ;i++)
    {
        int sLower1,sLower2;
        sLower1 = x_tolower(*p1);
        sLower2 = x_tolower(*p2);
        if ( sLower1 != sLower2)
        {
            return(sLower1 - sLower2);
        }
        ++p1;
        ++p2;
    }

    return ((i == nP1Len) && (0 == *p2)) ? 0 : (0 == *p2)? 1 : -1;

    // return ((0 == *p1) && (0 == *p2)) ? 0 : ((0 == *p1) ? -1 : 1);
}
