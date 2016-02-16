
#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_TableReaderWriter.h"

class MyDB_TableRecIterator : public MyDB_RecordIterator
{
public:
    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_TableRecIterator (MyDB_RecordPtr recordPtr, MyDB_TableReaderWriter * tableRWPtr);
    
    virtual ~MyDB_TableRecIterator () {};

private:

    MyDB_TableReaderWriter * tableReaderWriterPtr;

    MyDB_RecordPtr recordPtr;

    MyDB_RecordIteratorPtr pageRecordIterator;

    int pageIndex;
    
};

#endif