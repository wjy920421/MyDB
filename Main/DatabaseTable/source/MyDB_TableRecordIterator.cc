
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
    
    temp_counter = 0;

}

void MyDB_TableRecordIterator::getNext ()
{
    if (this->pageRecordIterator->hasNext())
    {
        this->pageRecordIterator->getNext();
        
        temp_counter++;
    }
    else if (this->pageIndex + 1 < this->tableReaderWriterPtr->pageVector.size())
    {
        //cout<<"page "<<pageIndex<<": "<<temp_counter<<endl;
        temp_counter = 0;
        
        
        this->pageRecordIterator = (*(this->tableReaderWriterPtr))[++ this->pageIndex].getIterator(this->recordPtr);
        this->getNext();
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
    }
    return true;
}


#endif