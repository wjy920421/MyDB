
#ifndef TABLE_REC_ITER_C
#define TABLE_REC_ITER_C

#include "MyDB_TableRecordIterator.h"
#include "MyDB_PageReaderWriter.h"


MyDB_TableRecordIterator::MyDB_TableRecordIterator (MyDB_RecordPtr recordPtr, MyDB_TableReaderWriter * tableRWPtr)
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
    else
    {
        this->pageIndex++;
        while (this->pageIndex <= this->tableReaderWriterPtr->tablePtr->lastPage())
        {
            if ((*(this->tableReaderWriterPtr))[this->pageIndex].getIterator(this->recordPtr)->hasNext())
            {
                this->pageRecordIterator = (*(this->tableReaderWriterPtr))[this->pageIndex].getIterator(this->recordPtr);
                this->pageRecordIterator->getNext();
                return;
            }
            this->pageIndex++;
        }
    }
}


bool MyDB_TableRecordIterator::hasNext ()
{
    if (! this->pageRecordIterator->hasNext())
    {
        int next = this->pageIndex + 1;
        while (next <= this->tableReaderWriterPtr->tablePtr->lastPage())
        {
            if ((*(this->tableReaderWriterPtr))[next].getIterator(this->recordPtr)->hasNext())
            {
                return true;
            }
            next++;
        }
        return false;
    }
    return true;
    
}


#endif