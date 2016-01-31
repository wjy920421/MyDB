
#ifndef ANONYMOUS_PAGE_C
#define ANONYMOUS_PAGE_C

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyDB_AnonymousPage.h"

using namespace std;


MyDB_AnonymousPage::MyDB_AnonymousPage(void * address, int size, string tempFile, bool pinned) : MyDB_Page(address, size, pinned)
{

    this->tempFile = tempFile;
}

void MyDB_AnonymousPage::release()
{
    MyDB_Page::release();

    if(this->referenceCounter == 0) this->wroteBytes();
}


void MyDB_AnonymousPage::wroteBytes()
{
    if (! this->isPinned())
    {
        int fd = open(this->tempFile.c_str(), O_CREAT | O_WRONLY | O_APPEND | O_FSYNC);
        if( fd == -1 || write(fd, this->pageAddress, this->pageSize) == -1 )
        {
            fprintf(stderr, "Failed to write to file");
            exit(-1);
        }
        if(close(fd) == -1)
        {
            fprintf(stderr, "Failed to close file");
        }
    }
}


void MyDB_AnonymousPage::loadBytes()
{

}


#endif


