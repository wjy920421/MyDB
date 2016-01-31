
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
    referenceCounter = 0;
    this->pageID = MyDB_Page::pageIDCounter++;
    this->pageAddress = address;
    this->pageSize = size;
    this->pinned = pinned;
}


void * MyDB_Page::getBytes()
{
    return this->pageAddress;
}

void MyDB_Page::wroteBytes()
{

}

void MyDB_Page::loadBytes()
{
    
}

int MyDB_Page::getPageID()
{
    return this->pageID;
}

#endif


