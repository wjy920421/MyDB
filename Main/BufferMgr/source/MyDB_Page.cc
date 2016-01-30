
#ifndef PAGE_C
#define PAGE_C

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyDB_Page.h"

using namespace std;


MyDB_Page::MyDB_Page(int pageID, void * address, int size, bool pinned)
{
    referenceCounter = 0;
    this->pageID = pageID;
    this->pageAddress = address;
    this->pageSize = size;
    this->isPinned = pinned;
}


void * MyDB_Page::getBytes()
{
    
}


void MyDB_Page::wroteBytes()
{

}


void MyDB_Page::loadBytes()
{

}


#endif


