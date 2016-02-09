
#ifndef PAGE_RW_H
#define PAGE_RW_H

#include "MyDB_PageType.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_Page.h"

class MyDB_PageReaderWriter {

public:
	MyDB_PageReaderWriter(MyDB_Page page = null);

    // ANY OTHER METHODS YOU WANT HERE

    // empties out the contents of this page, so that it has no records in it
    // the type of the page is set to MyDB_PageType :: RegularPage
    void clear ();    

    // return an itrator over this page... each time returnVal->next () is
    // called, the resulting record will be placed into the record pointed to
    // by iterateIntoMe
    MyDB_RecordIteratorPtr getIterator (MyDB_RecordPtr iterateIntoMe);

    // appends a record to this page... return false is the append fails because
    // there is not enough space on the page; otherwise, return true
    bool append (MyDB_RecordPtr appendMe);

    // gets the type of this page... this is just a value from an ennumeration
    // that is stored within the page
    MyDB_PageType getType ();

    // sets the type of the page
    void setType (MyDB_PageType toMe);

<<<<<<< HEAD
=======
    MyDB_PageReaderWriter(MyDB_Page page);
>>>>>>> 3739e4dad5770f14f8e081222176d9067978ecf4

    ~MyDB_PageReaderWriter();
    
private:

<<<<<<< HEAD
	MyDB_PageType pageType;
	MyDB_Page page;
	// ANYTHING ELSE YOU WANT HERE
=======
	MyDB_Page * page;
    
    MyDB_PageType pageType;
>>>>>>> 3739e4dad5770f14f8e081222176d9067978ecf4

};

#endif
