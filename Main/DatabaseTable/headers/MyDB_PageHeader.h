
#ifndef PAGE_HEADER_H
#define PAGE_HEADER_H

#include <memory>
#include "MyDB_PageType.h"
#include "MyDB_PageHandle.h"

typedef struct MyDB_PageHeaderStruct
{
    int dataSize;
    MyDB_PageType pageType;
    char data[0];
} MyDB_PageHeaderStruct;

class MyDB_PageHeader;
typedef shared_ptr <MyDB_PageHeader> MyDB_PageHeaderPtr;

class MyDB_PageHeader
{
public:

    MyDB_PageHeader(MyDB_PageHandle pageHandle) { this->pageHandle = pageHandle; }

    // Get data head in page header
    char * getData()
    {
        MyDB_PageHeaderStruct * pageHeader = (MyDB_PageHeaderStruct *)this->pageHandle->getBytes();
        return pageHeader->data;
    }

    // Set the data size in page header
    int getDataSize()
    {
        MyDB_PageHeaderStruct * pageHeader = (MyDB_PageHeaderStruct *)this->pageHandle->getBytes();
        return pageHeader->dataSize;
    }

    // Set the data size in page header
    void setDataSize(int size)
    {
        MyDB_PageHeaderStruct * pageHeader = (MyDB_PageHeaderStruct *)this->pageHandle->getBytes();
        pageHeader->dataSize = size;
    }

    MyDB_PageType getPageType()
    {
        MyDB_PageHeaderStruct * pageHeader = (MyDB_PageHeaderStruct *)this->pageHandle->getBytes();
        return pageHeader->pageType;
    }

    void setPageType(MyDB_PageType pageType)
    {
        MyDB_PageHeaderStruct * pageHeader = (MyDB_PageHeaderStruct *)this->pageHandle->getBytes();
        pageHeader->pageType = pageType;
    }

private:

    MyDB_PageHandle pageHandle;

};


#endif
