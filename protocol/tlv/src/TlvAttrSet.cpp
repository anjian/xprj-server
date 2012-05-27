#include <protocol/tlv/inc/TlvAttrSet.h>

////////////////////////////////////////////////////////////////////////////////
// get set iterator
////////////////////////////////////////////////////////////////////////////////
TlvAttrIterator_i* TlvAttrSet_c::getIterator()
{
    TlvAttrSetIterator_c* pIterator = new TlvAttrSetIterator_c(this);
    if (NULL != pIterator)
    {
        pIterator->setList(&lstAttrs_m);
    }

    return pIterator;
}
