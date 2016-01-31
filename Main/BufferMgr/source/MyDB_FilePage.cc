
#ifndef FILE_PAGE_C
#define FILE_PAGE_C

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyDB_FilePage.h"

using namespace std;


MyDB_FilePage::MyDB_FilePage(void * address, int size, MyDB_TablePtr table, int index, bool pinned): MyDB_Page(address, size, pinned)
{
    this->table = table;
    this->tableIndex = index;

    this->loadBytes();
}


void MyDB_FilePage::wroteBytes()
{
    if (! this->isPinned())
    {
        int fd = open(this->table->getStorageLoc().c_str(), O_CREAT | O_WRONLY | O_FSYNC);
        if( fd == -1 || lseek(fd, this->tableIndex * this->pageSize, SEEK_SET) == -1 || write(fd, this->pageAddress, this->pageSize) == -1 )
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


void MyDB_FilePage::loadBytes()
{
    int fd = open(this->table->getStorageLoc().c_str(), O_RDONLY | O_FSYNC);
    if( fd == -1 || lseek(fd, this->tableIndex * this->pageSize, SEEK_SET) == -1 || read(fd, this->pageAddress, pageSize) == -1 )
    {
        fprintf(stderr, "Failed to read from file");
        exit(-1);
    }
    
    close(fd);
}


#endif


