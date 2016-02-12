
#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_TableRecordIterator.h"
#include "MyDB_PageReaderWriter.h"


MyDB_TableRecordIterator::MyDB_TableRecordIterator (MyDB_RecordPtr recordPtr, MyDB_TableReaderWriterPtr tableRWPtr)
{
    this->recordPtr = recordPtr;
    this->tableReaderWriterPtr = tableRWPtr;
    this->pageIndex = 0;
    this->pageRecordIterator = (*(this->tableReaderWriterPtr))[0].getIterator(this->recordPtr);
}

void MyDB_TableRecordIterator::getNext ()
{
    if (this->pageRecordIterator->hasNext())
    {
        this->pageRecordIterator->getNext();
    }
    else if (this->pageIndex + 1 < this->tableReaderWriterPtr->pageVector.size())
    {
        this->pageRecordIterator = (*(this->tableReaderWriterPtr))[++ this->pageIndex].getIterator(this->recordPtr);
        this->pageRecordIterator->getNext();
    }
}


bool MyDB_TableRecordIterator::hasNext ()
{
    if (! this->pageRecordIterator->hasNext())
    {
        if (this->pageIndex + 1 >= this->tableReaderWriterPtr->pageVector.size())
        {
            return false;
        }
        this->pageIndex ++;
    }
    return true;
}


#endif