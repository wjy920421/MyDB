
#ifndef TABLE_REC_ITER_H
#define TABLE_REC_ITER_H

#include "MyDB_RecordIterator.h"

class MyDB_TableRecordIterator : public MyDB_RecordIterator
{
public:
    virtual void getNext ();

    virtual bool hasNext ();

    MyDB_TableRecordIterator () {};
    
    virtual ~MyDB_TableRecordIterator () {};
};

#endif