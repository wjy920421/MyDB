
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecIterator.h"
#include "MyDB_PageRecIteratorAlt.h"
#include "MyDB_PageListIteratorAlt.h"
#include "RecordComparator.h"


MyDB_RecordIteratorAltPtr getIteratorAlt (vector <MyDB_PageReaderWriter> &forUs)
{
    return make_shared <MyDB_PageListIteratorAlt> (forUs);
}


MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, bool clear)
{
    this->pageHandle = pageHandle;
    this->pageHeader = make_shared <MyDB_PageHeader> (this->pageHandle);
    this->setType(MyDB_PageType::RegularPage);
    
    if (clear) this->pageHeader->setDataSize(0);
    //if (clear) this->pageHandle->wroteBytes();
}


void MyDB_PageReaderWriter::clear ()
{
    this->pageHeader->setDataSize(0);
    this->pageHandle->wroteBytes();
    this->setType(MyDB_PageType::RegularPage);
}


MyDB_PageType MyDB_PageReaderWriter::getType ()
{
    return this->pageHeader->getPageType();
}


MyDB_RecordIteratorPtr MyDB_PageReaderWriter::getIterator (MyDB_RecordPtr record)
{
    return make_shared <MyDB_PageRecIterator> (record, this);;
}


MyDB_RecordIteratorAltPtr MyDB_PageReaderWriter::getIteratorAlt ()
{
    return make_shared <MyDB_PageRecIteratorAlt> (this->pageHandle);
}


void MyDB_PageReaderWriter::setType (MyDB_PageType toMe)
{
    this->pageHeader->setPageType(toMe);
}


bool MyDB_PageReaderWriter::append (MyDB_RecordPtr record)
{
    char * newLocation = this->pageHeader->getData() + this->pageHeader->getDataSize() + record->getBinarySize();
    if (newLocation <= (char *)this->pageHandle->getBytes() + this->pageHandle->getSize())
    {
        char * tail = (char *)record->toBinary(this->pageHeader->getData() + this->pageHeader->getDataSize());
        this->pageHeader->setDataSize(tail - this->pageHeader->getData());
        this->pageHandle->wroteBytes();
        
        return true;
    }

    return false;
}


MyDB_PageReaderWriterPtr MyDB_PageReaderWriter::sort (function <bool()> comparator, MyDB_RecordPtr lhs,  MyDB_RecordPtr rhs) {

    // first, read in the positions of all of the records
    vector <char *> positions;
    
    // this basically iterates through all of the records on the page
    int bytesConsumed = 0;
    while (bytesConsumed < this->pageHeader->getDataSize())
    {
        char * pos = bytesConsumed + this->pageHeader->getData();
        positions.push_back (pos);
        void * nextPos = lhs->fromBinary (pos);
        bytesConsumed += (char *)nextPos - (char *)pos;
    }

    // and now we sort the vector of positions, using the record contents to build a comparator
    RecordComparator myComparator(comparator, lhs, rhs);
    std::sort(positions.begin(), positions.end(), myComparator);

    // and now create the page to return
    MyDB_PageReaderWriterPtr returnVal = make_shared <MyDB_PageReaderWriter> (this->pageHandle);
    returnVal->clear();
    
    // loop through all of the sorted records and write them out
    for (void * pos : positions)
    {
        lhs->fromBinary(pos);
        returnVal->append(lhs);
    }

    return returnVal;
}


#endif


