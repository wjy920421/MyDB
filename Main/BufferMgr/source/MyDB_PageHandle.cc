
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"


MyDB_PageHandleBase::MyDB_PageHandleBase(MyDB_Page * page)
{
    this->page = page;
    if(this->page != nullptr)
    {
        this->page->retain();
        this->pageID = page->getPageID();
    }
}


MyDB_PageHandleBase::~MyDB_PageHandleBase ()
{
    if(this->page != nullptr) this->page->release();
}


void * MyDB_PageHandleBase::getBytes ()
{
    if(this->page == nullptr)
    {
        fprintf(stderr, "The page handler is not pointing to any pages");
        return nullptr;
    }

    return this->page->getBytes();
}

void MyDB_PageHandleBase::wroteBytes ()
{
    if(this->page == nullptr)
    {
        fprintf(stderr, "The page handler is not pointing to any pages");
        return;
    }

    this->page->wroteBytes();
}


#endif

