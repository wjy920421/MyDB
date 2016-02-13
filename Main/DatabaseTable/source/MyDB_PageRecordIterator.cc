
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"


MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter * pageRWPtr)
{
    this->recordPtr = recordPtr;
    this->pageReaderWriterPtr = pageRWPtr;
    this->visitedBytes = 0;
}


void MyDB_PageRecordIterator::getNext ()
{
    if (this->hasNext())
    {
        this->recordPtr->fromBinary(this->pageReaderWriterPtr->getPageHeaderData() + this->visitedBytes);
        this->visitedBytes += this->recordPtr->getBinarySize();
    }
}


bool MyDB_PageRecordIterator::hasNext ()
{
    if (this->visitedBytes < this->pageReaderWriterPtr->getPageHeaderDataSize())
    {
        return true;
    }

    return false;
}


#endif