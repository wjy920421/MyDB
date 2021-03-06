
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_PageReaderWriter.h"


class MyDB_PageRecIterator : public MyDB_RecordIterator
{
public:

    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_PageRecIterator (MyDB_RecordPtr record, MyDB_PageReaderWriter * pageRWPtr);
    
    virtual ~MyDB_PageRecIterator () {}

private:

    MyDB_PageReaderWriter * pageReaderWriterPtr;

    MyDB_RecordPtr recordPtr;

    int visitedBytes;

};

#endif