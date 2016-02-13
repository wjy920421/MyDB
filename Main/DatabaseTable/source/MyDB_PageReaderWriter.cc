
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_PageRecordIterator.h"



MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_PageHandle pageHandle, bool clear)
{
    this->pageHandle = pageHandle;
    this->pageType = MyDB_PageType::RegularPage;
    
    if (clear) this->setPageHeaderDataSize(0);
    //if (clear) this->pageHandle->wroteBytes();
}


void MyDB_PageReaderWriter::setPageHeaderDataSize(int size)
{
    MyDB_PageHeader * pageHeader = (MyDB_PageHeader *)this->pageHandle->getBytes();
    pageHeader->dataSize = size;
}


int MyDB_PageReaderWriter::getPageHeaderDataSize()
{
    MyDB_PageHeader * pageHeader = (MyDB_PageHeader *)this->pageHandle->getBytes();
    return pageHeader->dataSize;
}


char * MyDB_PageReaderWriter::getPageHeaderData()
{
    MyDB_PageHeader * pageHeader = (MyDB_PageHeader *)this->pageHandle->getBytes();
    return pageHeader->data;
}


void MyDB_PageReaderWriter::clear ()
{
    this->setPageHeaderDataSize(0);
    this->pageHandle->wroteBytes();
    this->pageType = MyDB_PageType::RegularPage;
}


MyDB_PageType MyDB_PageReaderWriter::getType ()
{
    return this->pageType;
}


MyDB_RecordIteratorPtr MyDB_PageReaderWriter::getIterator (MyDB_RecordPtr record)
{
    return make_shared <MyDB_PageRecordIterator> (record, this);;
}



void MyDB_PageReaderWriter::setType (MyDB_PageType toMe)
{
    this->pageType = toMe;
}


bool MyDB_PageReaderWriter::append (MyDB_RecordPtr record)
{
    char * newLocation = this->getPageHeaderData() + this->getPageHeaderDataSize() + record->getBinarySize();
    if (newLocation <= (char *)this->pageHandle->getBytes() + this->pageHandle->getSize())
    {
        char * tail = (char *)record->toBinary(this->getPageHeaderData() + this->getPageHeaderDataSize());
        this->setPageHeaderDataSize(tail - this->getPageHeaderData());
        this->pageHandle->wroteBytes();
        
        return true;
    }

    return false;
}

#endif


