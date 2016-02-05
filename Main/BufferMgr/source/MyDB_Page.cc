
#ifndef PAGE_C
#define PAGE_C

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyDB_Page.h"

using namespace std;


int MyDB_Page::pageIDCounter = 0;


MyDB_Page::MyDB_Page(void * address, int size, bool pinned)
{
    this->referenceCounter = 0;
    this->pageID = to_string(MyDB_Page::pageIDCounter++);
    this->pageAddress = address;
    this->pageSize = size;
    this->pinned = pinned;
    this->dirty = false;
    this->evicted = false;
}


void * MyDB_Page::getBytes()
{
    if(this->evicted)
    {
        this->bufferManagerDelegate.reload(this);
        this->loadFromFile();
    }

    return this->pageAddress;
}


string MyDB_Page::getPageID()
{
    return this->pageID;
}

#endif


