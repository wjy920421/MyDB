
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
    this->pageID = this->pageID + "_anonymous_page";
    this->tempFile = tempFile;
}


MyDB_AnonymousPage::~MyDB_AnonymousPage()
{
    if(!this->evicted) this->writeToFile();
}

void MyDB_AnonymousPage::release()
{
    MyDB_Page::release();

    if(this->referenceCounter == 0)
    {
        this->delegateRelease(this->pageID);
        delete this;
    }
}


void MyDB_AnonymousPage::writeToFile()
{
    if (!this->isPinned() && this->dirty)
    {
        int fd = open(this->tempFile.c_str(), O_CREAT | O_RDWR | O_APPEND | O_FSYNC, S_IRWXU);
        if( fd == -1 || write(fd, this->pageAddress, this->pageSize) == -1 )
        {
            fprintf(stderr, "Failed to write page \'%s\' to \'%s\'", this->pageID.c_str(), this->tempFile.c_str());
            exit(-1);
        }
        if(close(fd) == -1)
        {
            fprintf(stderr, "Failed to close \'%s\'", this->tempFile.c_str());
        }
    }
}


#endif


