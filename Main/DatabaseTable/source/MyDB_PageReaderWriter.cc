
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"



MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, MyDB_SchemaPtr schema)
{
    this->pageHandle = pageHandle;
    this->pageType = MyDB_PageType::RegularPage;
    this->schema = schema;

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
    void * newLocation = this->currentLocation + recordSize;
    if (newLocation <= this->pageHandle->getBytes() + this->pageHandle->getSize())
    {
        this->currentLocation = record->toBinary(this->currentLocation);
        return true;
    }

	return false;
}

#endif


