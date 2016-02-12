
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_RecordIterator.h"

class MyDB_PageRecordIterator : public MyDB_RecordIterator
{
public:

    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_PageRecordIterator (MyDB_RecordPtr record, MyDB_PageReaderWriter & pageRW);
    
    virtual ~MyDB_PageRecordIterator ();

private:

    MyDB_PageReaderWriter & pageReaderWriter;

    void * data;

    size_t size;



};

#endif