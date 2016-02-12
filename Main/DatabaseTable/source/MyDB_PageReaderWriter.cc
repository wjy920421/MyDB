
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecordIterator.h"



MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_PageHandle pageHandle)
{
    this->pageHandle = pageHandle;
    this->pageType = MyDB_PageType::RegularPage;

    this->pageHeader = (MyDB_PageHeader *)this->pageHandle->getBytes();
}



void MyDB_PageReaderWriter::clear ()
{
	this->pageHeader->dataSize = 0;
}


MyDB_PageType MyDB_PageReaderWriter::getType ()
{
	return this->pageType;
}


MyDB_RecordIteratorPtr MyDB_PageReaderWriter::getIterator (MyDB_RecordPtr record)
{
	return make_shared <MyDB_PageRecordIterator> (record, *this);;
}



void MyDB_PageReaderWriter::setType (MyDB_PageType toMe)
{
    this->pageType = toMe;
}


bool MyDB_PageReaderWriter::append (MyDB_RecordPtr record)
{
    size_t recordSize = record->getBinarySize();
    char * newLocation = this->currentLocation + recordSize;
    if (newLocation <= (char *)this->pageHandle->getBytes() + this->pageHandle->getSize())
    {
        this->currentLocation = (char *)record->toBinary(this->currentLocation);
        this->pageHeader->dataSize = this->currentLocation - this->pageHeader->data;
        return true;
    }

	return false;
}

#endif


