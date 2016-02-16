
#ifndef PAGE_RW_H
#define PAGE_RW_H

#include "MyDB_Page.h"
#include "MyDB_Schema.h"
#include "MyDB_PageHandle.h"
#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_RecordIteratorAlt.h"
#include "MyDB_PageHeader.h"
#include "MyDB_BufferManager.h"


class MyDB_PageReaderWriter;
typedef shared_ptr <MyDB_PageReaderWriter> MyDB_PageReaderWriterPtr;


class MyDB_PageReaderWriter {

public:

    // ANY OTHER METHODS YOU WANT HERE

    // empties out the contents of this page, so that it has no records in it
    // the type of the page is set to MyDB_PageType :: RegularPage
    void clear ();    

    // return an itrator over this page... each time returnVal->next () is
    // called, the resulting record will be placed into the record pointed to
    // by iterateIntoMe
    MyDB_RecordIteratorPtr getIterator (MyDB_RecordPtr iterateIntoMe);

    // gets an instance of an alternate iterator over the page... this is an
    // iterator that has the alternate getCurrent ()/advance () interface
    MyDB_RecordIteratorAltPtr getIteratorAlt ();

    // gets an instance of an alternatie iterator over a list of pages
    friend MyDB_RecordIteratorAltPtr getIteratorAlt (vector <MyDB_PageReaderWriter> &forUs);

    // appends a record to this page... return false is the append fails because
    // there is not enough space on the page; otherwise, return true
    bool append (MyDB_RecordPtr appendMe);

    // gets the type of this page... this is just a value from an ennumeration
    // that is stored within the page
    MyDB_PageType getType ();

    // sets the type of the page
    void setType (MyDB_PageType toMe);
    
    // Constructor
    MyDB_PageReaderWriter(MyDB_BufferManagerPtr bufferManagerPtr, MyDB_PageHandle pageHandle, bool clear = false);
    
    // Destructor
    ~MyDB_PageReaderWriter() {}

    // sorts the contents of the page... the boolean lambda that is sent into
    // this function must check to see if the contents of the record pointed to
    // by lhs are less than the contens of the record pointed to by rhs... typically,
    // this lambda would have been created via a call to buildRecordComparator
    MyDB_PageReaderWriterPtr sort (function <bool ()> comparator, MyDB_RecordPtr lhs,  MyDB_RecordPtr rhs);
    
private:
    
    // Friends with MyDB_PageRecIterator
    friend class MyDB_PageRecIterator;
    
    // Handle to the page in buffer
    MyDB_PageHandle pageHandle;
    
    // Header of the page
    MyDB_PageHeaderPtr pageHeader;
    
    // Buffer manager
    MyDB_BufferManagerPtr bufferManagerPtr;
};

#endif
