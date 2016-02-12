
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"


MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter * pageRWPtr)
{
    this->recordPtr = recordPtr;
    this->pageReaderWriterPtr = pageRWPtr;
    this->currentLocation = this->pageReaderWriterPtr->getPageHeaderData();
}


void MyDB_PageRecordIterator::getNext ()
{
    if (this->hasNext())
    {
        this->currentLocation = (char *)this->recordPtr->fromBinary(this->currentLocation);
    }
}


bool MyDB_PageRecordIterator::hasNext ()
{
    char * tail = this->pageReaderWriterPtr->getPageHeaderData() + this->pageReaderWriterPtr->getPageHeaderDataSize();

    if (this->currentLocation < tail)
    {
        return true;
    }

    return false;
}


#endif