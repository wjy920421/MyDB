
#ifndef PAGE_REC_ITER_H
#define PAGE_REC_ITER_H

#include "MyDB_RecordIterator.h"

class MyDB_PageRecordIterator : public MyDB_RecordIterator
{
public:
    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_PageRecordIterator () {};
    
    virtual ~MyDB_PageRecordIterator () {};
};

#endif