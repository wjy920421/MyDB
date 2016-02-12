
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"


MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter & pageRW)
: pageReaderWriter(pageRW)
{
    this->recordPtr = recordPtr;
}


void MyDB_PageRecordIterator::getNext ()
{
    if (this->hasNext())
    {
        this->pageReaderWriter.currentLocation = (char *)this->recordPtr->fromBinary(this->pageReaderWriter.currentLocation);
        this->pageReaderWriter.pageHeader->dataSize = this->pageReaderWriter.currentLocation - this->pageReaderWriter.pageHeader->data;
    }
}


bool MyDB_PageRecordIterator::hasNext ()
{
    char * currentLocation = this->pageReaderWriter.currentLocation;
    char * maxLocation = this->pageReaderWriter.pageHeader->data + this->pageReaderWriter.pageHeader->dataSize;

    if (currentLocation < maxLocation)
    {
        return true;
    }

    return false;
}


#endif