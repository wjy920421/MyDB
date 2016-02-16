
#ifndef TABLE_RW_H
#define TABLE_RW_H

#include <memory>
#include <vector>
#include "MyDB_BufferManager.h"
#include "MyDB_Table.h"
#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_RecordIteratorAlt.h"


// create a smart pointer for the catalog
using namespace std;
class MyDB_PageReaderWriter;
class MyDB_TableReaderWriter;
typedef shared_ptr <MyDB_TableReaderWriter> MyDB_TableReaderWriterPtr;

class MyDB_TableReaderWriter {

public:

    // ANYTHING ELSE YOU NEED HERE

    // create a table reader/writer for the specified table, using the specified
    // buffer manager
    MyDB_TableReaderWriter (MyDB_TablePtr forMe, MyDB_BufferManagerPtr myBuffer);

    // gets an empty record from this table
    MyDB_RecordPtr getEmptyRecord ();

    // append a record to the table
    void append (MyDB_RecordPtr appendMe);

    // return an itrator over this table... each time returnVal->next () is
    // called, the resulting record will be placed into the record pointed to
    // by iterateIntoMe
    MyDB_RecordIteratorPtr getIterator (MyDB_RecordPtr iterateIntoMe);

    // gets an instance of an alternate iterator over the table... this is an
    // iterator that has the alternate getCurrent ()/advance () interface
    MyDB_RecordIteratorAltPtr getIteratorAlt ();

    // gets an instance of an alternate iterator over the page; this iterator
    // works on a range of pages in the file, and iterates from lowPage through
    // highPage inclusive
    MyDB_RecordIteratorAltPtr getIteratorAlt (int lowPage, int highPage);

    // load a text file into this table... overwrites the current contents
    void loadFromTextFile (string fromMe);

    // dump the contents of this table into a text file
    void writeIntoTextFile (string toMe);

    // access the i^th page in this file
    MyDB_PageReaderWriter &operator [] (size_t i);

    // access the last page in the file
    MyDB_PageReaderWriter &last ();

    // get the number of pages in the file
    int getNumPages ();

    // get access to the buffer manager 
    MyDB_BufferManagerPtr getBufferMgr ();

private:
    
    // Friends with MyDB_TableRecIterator
    friend class MyDB_TableRecIterator;
    
    // Buffer manager that manages pages in a buffer pool
    MyDB_BufferManagerPtr bufferManagerPtr;
    
    // Table to be processed
    MyDB_TablePtr tablePtr;
    
    // Vector of pages
    vector<MyDB_PageReaderWriter> pageVector;
    
};

#endif
