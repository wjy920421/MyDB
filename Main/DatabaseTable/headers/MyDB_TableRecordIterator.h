
#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_TableReaderWriter.h"

class MyDB_TableRecordIterator : public MyDB_RecordIterator
{
public:
    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_TableRecordIterator (MyDB_RecordPtr recordPtr, MyDB_TableReaderWriterPtr tableRWPtr);
    
    virtual ~MyDB_TableRecordIterator () {};

private:

    MyDB_TableReaderWriterPtr tableReaderWriterPtr;

    MyDB_RecordPtr recordPtr;

    MyDB_RecordIteratorPtr pageRecordIterator;

    int pageIndex;
};

#endif