
#ifndef PAGE_REC_ITER_ALT_C
#define PAGE_REC_ITER_ALT_C

#include "MyDB_PageRecIteratorAlt.h"


void MyDB_PageRecIteratorAlt::getCurrent (MyDB_RecordPtr intoMe)
{
    void * pos = bytesConsumed + pageHeader->getData();
    void * nextPos = intoMe->fromBinary(pos);
    nextRecSize = ((char *)nextPos) - ((char *)pos);    
}

bool MyDB_PageRecIteratorAlt::advance ()
{
    if (nextRecSize == -1) {
        cout << "You can't call advance without calling getCurrent!!\n";
        exit (1);
    }
    bytesConsumed += nextRecSize;
    nextRecSize = -1;
    return bytesConsumed != pageHeader->getDataSize();
}

MyDB_PageRecIteratorAlt::MyDB_PageRecIteratorAlt (MyDB_PageHandle myPageIn)
{
    bytesConsumed = 0;
    myPage = myPageIn;
    nextRecSize = 0;
    pageHeader = make_shared <MyDB_PageHeader> (myPage);
}

MyDB_PageRecIteratorAlt::~MyDB_PageRecIteratorAlt () {}

#endif
