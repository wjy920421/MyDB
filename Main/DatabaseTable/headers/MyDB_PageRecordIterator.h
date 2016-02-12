
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_PageReaderWriter.h"

class MyDB_PageRecordIterator : public MyDB_RecordIterator
{
public:

    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_PageRecordIterator (MyDB_RecordPtr record, MyDB_PageReaderWriter & pageRW);
    
    virtual ~MyDB_PageRecordIterator ();

private:

    MyDB_PageReaderWriter & pageReaderWriter;

    MyDB_RecordPtr recordPtr;

};

#endif