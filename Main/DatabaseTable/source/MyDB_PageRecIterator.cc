
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecIterator.h"


MyDB_PageRecIterator::MyDB_PageRecIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter * pageRWPtr)
{
    this->recordPtr = recordPtr;
    this->pageReaderWriterPtr = pageRWPtr;
    this->visitedBytes = 0;
}


void MyDB_PageRecIterator::getNext ()
{
    if (this->hasNext())
    {
        this->recordPtr->fromBinary(this->pageReaderWriterPtr->pageHeader->getData() + this->visitedBytes);
        this->visitedBytes += this->recordPtr->getBinarySize();
    }
}


bool MyDB_PageRecIterator::hasNext ()
{
    if (this->visitedBytes < this->pageReaderWriterPtr->pageHeader->getDataSize())
    {
        return true;
    }

    return false;
}


#endif