
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"



MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, MyDB_SchemaPtr schema)
{
    this->pageHandle = pageHandle;
    this->pageType = MyDB_PageType::RegularPage;
    this->schema = schema;
}



void MyDB_PageReaderWriter :: clear () {
	
}


MyDB_PageType MyDB_PageReaderWriter :: getType () {
	return this->pageType;
}


MyDB_RecordIteratorPtr MyDB_PageReaderWriter::getIterator (MyDB_RecordPtr)
{
	return nullptr;
}



void MyDB_PageReaderWriter::setType (MyDB_PageType toMe)
{
    this->pageType = toMe;
}


bool MyDB_PageReaderWriter::append (MyDB_RecordPtr)
{
	return true;
}

#endif


