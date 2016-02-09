
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"


<<<<<<< HEAD
MyDB_PageReaderWriter :: MyDB_PageReaderWriter(MyDB_Page page){
	this->page = page;
	this->pageType = MyDB_PageType :: RegularPage;
=======
MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_Page page)
{
    this->page = page;
    this->pageType = MyDB_PageType::RegularPage;
}


void MyDB_PageReaderWriter::clear ()
{

>>>>>>> 3739e4dad5770f14f8e081222176d9067978ecf4
}

void MyDB_PageReaderWriter :: clear () {
	
}

<<<<<<< HEAD
MyDB_PageType MyDB_PageReaderWriter :: getType () {
	return this->pageType;
=======
MyDB_PageType MyDB_PageReaderWriter::getType ()
{
	return MyDB_PageType::RegularPage;
>>>>>>> 3739e4dad5770f14f8e081222176d9067978ecf4
}


MyDB_RecordIteratorPtr MyDB_PageReaderWriter::getIterator (MyDB_RecordPtr)
{
	return nullptr;
}


<<<<<<< HEAD
void MyDB_PageReaderWriter :: setType (MyDB_PageType pageType) {
	this->pageType = pageType;
=======
void MyDB_PageReaderWriter::setType (MyDB_PageType toMe)
{
    this->pageType = toMe;
>>>>>>> 3739e4dad5770f14f8e081222176d9067978ecf4
}


bool MyDB_PageReaderWriter::append (MyDB_RecordPtr)
{
	return true;
}


#endif


