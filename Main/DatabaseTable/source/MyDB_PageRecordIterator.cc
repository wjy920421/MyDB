
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"


MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter * pageRWPtr)
{
    this->recordPtr = recordPtr;
    this->pageReaderWriterPtr = pageRWPtr;
}


void MyDB_PageRecordIterator::getNext ()
{
    if (this->hasNext())
    {
        this->pageReaderWriterPtr->currentLocation = (char *)this->recordPtr->fromBinary(this->pageReaderWriterPtr->currentLocation);
        this->pageReaderWriterPtr->pageHeader->dataSize = this->pageReaderWriterPtr->currentLocation - this->pageReaderWriterPtr->pageHeader->data;
    }
}


bool MyDB_PageRecordIterator::hasNext ()
{
    char * currentLocation = this->pageReaderWriterPtr->currentLocation;
    char * maxLocation = this->pageReaderWriterPtr->pageHeader->data + this->pageReaderWriterPtr->pageHeader->dataSize;

    if (currentLocation < maxLocation)
    {
        return true;
    }

    return false;
}


#endif