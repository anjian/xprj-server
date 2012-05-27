
#include <interface/protocol/TlvIf.h>

#include <protocol/tlv/inc/TlvEncoder.h>

////////////////////////////////////////////////////////////////////////////////
// TlvAttrStackItem_c implementation
////////////////////////////////////////////////////////////////////////////////
TlvAttrStackItem_c::~TlvAttrStackItem_c()
{
    if (NULL != pIterator_m)
    {
        delete pIterator_m;
        pIterator_m = NULL;
    }
}

bool TlvAttrStackItem_c::appendAttr(TlvAttrIf_i* pItem)
{
    if ((NULL == pItem) || (!pItem->isAtomic()))
    {
        return false;
    }

    TlvGenerator_c::appendType(tbBuffer_m, (TlvAttrType_e)pItem->getType());
    TlvGenerator_c::appendLength(tbBuffer_m, pItem->getValueLen());
    TlvGenerator_c::appendValue(tbBuffer_m, pItem->getValue(), pItem->getValueLen());

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// TlvEncoder_c implementation
////////////////////////////////////////////////////////////////////////////////
TlvEncoder_c::~TlvEncoder_c()
{
    for (int nIndex=lstAttrSetStack_m.size()-1; nIndex>=0; nIndex--)
    {
        delete lstAttrSetStack_m.takeLast();
    }
}

bool TlvEncoder_c::generate(TempSimpleVector_c<TlvAttrIf_i*>* ppAttr, TempMemBuffer_c& tbOutput)
{
    for (int nIndex=0; nIndex<ppAttr->size(); nIndex++)
    {
        if (!generate(ppAttr->get(nIndex), tbOutput))
        {
            return false;
        }
    }

    return true;
}

bool TlvEncoder_c::generate(TlvAttrIf_i* pAttr, TempMemBuffer_c& tbOutput)
{
    if (NULL == pAttr)
    {
        return false;
    }

    TlvAttrIf_i* pItem = pAttr;

    while ((lstAttrSetStack_m.size() > 0) || (NULL != pItem))
    {
        TlvAttrStackItem_c* pStackTop = (lstAttrSetStack_m.size() > 0) ? lstAttrSetStack_m.getLast() : NULL;
        TlvAttrIterator_i* pIterator  = (NULL != pStackTop) ? pStackTop->getIterator() : NULL;

        if (NULL == pItem)
        {
            // In this case , iterator must not be null
            pItem = pIterator->next();
        }

        if (NULL != pItem)
        {
            if (pItem->isAtomic())
            {
                if (NULL != pStackTop)
                {
                    pStackTop->appendAttr(pItem);
                }
                else
                {
                    TlvGenerator_c::appendType(tbOutput, (TlvAttrType_e)pItem->getType());
                    TlvGenerator_c::appendLength(tbOutput, pItem->getValueLen());
                    TlvGenerator_c::appendValue(tbOutput, pItem->getValue(), pItem->getValueLen());
                }
            }
            else
            {
                // create stack item, and push it into stack
                // 1. create stack item
                TlvAttrStackItem_c* pStackItem = new TlvAttrStackItem_c();
                if (NULL == pStackItem)
                {
                    return false;
                }

                TlvAttrIterator_i* pIterator = pItem->getIterator();
                if (NULL == pIterator)
                {
                    delete pStackItem;
                    return false;
                }


                pStackItem->setItem(pItem);
                pStackItem->setIterator(pIterator);

                // 2. push into stack
                lstAttrSetStack_m.append(pStackItem);
            }

            pItem = NULL;
        }
        else
        {
            // all children have been processed, pop it up now
            // In this case, there is at least one item in stack
            TlvAttrStackItem_c* pStackDone  = lstAttrSetStack_m.takeLast();

            if (lstAttrSetStack_m.size() > 0)
            {
                TlvAttrStackItem_c* pStackTop   = lstAttrSetStack_m.getLast();

                pStackTop->appendType(pStackDone->getItem()->getType());
                pStackTop->appendLength(pStackDone->getAttrLength());
                pStackTop->append(pStackDone->getAttrValue(), pStackDone->getAttrLength());
            }
            else
            {
                // there is no item in stack, and this should be the last attribute in this around,
                // append it into output buffer directly
                TlvGenerator_c::appendType(tbOutput, (TlvAttrType_e)pStackDone->getItem()->getType());
                TlvGenerator_c::appendLength(tbOutput, pStackDone->getAttrLength());
                TlvGenerator_c::appendValue(tbOutput, pStackDone->getAttrValue(), pStackDone->getAttrLength());
            }

            delete pStackDone;
        }
    }

    return true;
}
