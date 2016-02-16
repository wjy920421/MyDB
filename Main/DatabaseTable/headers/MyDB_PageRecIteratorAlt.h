
#ifndef PAGE_REC_ITER_ALT_H
#define PAGE_REC_ITER_ALT_H

#include "MyDB_PageHandle.h"
#include "MyDB_Record.h"
#include "MyDB_RecordIteratorAlt.h"
#include "MyDB_PageHeader.h"

class MyDB_PageRecIteratorAlt : public MyDB_RecordIteratorAlt {

public:

        // load the current record into the parameter
        void getCurrent (MyDB_RecordPtr intoMe) override;

        // advance to the next record... returns true if there is a next record, and
        // false if there are no more records to iterate over.  Not that this cannot
        // be called until after getCurrent () has been called
        bool advance () override;

	// destructor and contructor
	MyDB_PageRecIteratorAlt (MyDB_PageHandle myPageIn); 
	~MyDB_PageRecIteratorAlt ();

private:

	int bytesConsumed;
	int nextRecSize;
	MyDB_PageHandle myPage;
    MyDB_PageHeaderPtr pageHeader;
};

#endif
