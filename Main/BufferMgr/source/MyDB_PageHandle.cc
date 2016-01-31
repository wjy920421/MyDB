
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "MyDB_PageHandle.h"

/*
void MyDB_PageHandleBase::MyDB_PageHandleBase(MyDB_Page * page)
{
    this->page = page;
}


MyDB_PageHandleBase :: ~MyDB_PageHandleBase ()
{

}
*/

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

