
#ifndef PAGE_REC_ITER_C
#define PAGE_REC_ITER_C

#include "MyDB_PageRecordIterator.h"


MyDB_PageRecordIterator::MyDB_PageRecordIterator (MyDB_RecordPtr recordPtr, MyDB_PageReaderWriter & pageRW)
: pageReaderWriter(pageRW)
{
    //data = recordPtr->
}


MyDB_PageRecordIterator::~MyDB_PageRecordIterator ()
{

}


void MyDB_PageRecordIterator::getNext ()
{

}


bool MyDB_PageRecordIterator::hasNext ()
{
    return false;
}


#endif