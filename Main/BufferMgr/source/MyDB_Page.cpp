
#ifndef PAGE_C
#define PAGE_C

#include <memory>
#include "MyDB_Page.h"

using namespace std;


MyDB_Page::MyDB_Page(void * address, int size)
{
    this->pageAddress = address;
    this->pageSize = size;
}


void * getBytes()
{
    return nullptr;
}
    

void wroteBytes()
{

}


#endif