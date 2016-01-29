
#ifndef PAGE_C
#define PAGE_C

#include <memory>
#include "MyDB_Page.h"

using namespace std;


MyDB_Page::MyDB_Page(void * address, int size)
{
    this->pageAddress = address;
    this->pageSize = size;
    this->isPinned = false;
}


void * getBytes()
{
    return this->pageAddress;
}
    

void wroteBytes()
{
    if (! isPinned)
    {

    }
}


#endif