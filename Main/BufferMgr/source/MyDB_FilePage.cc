
#ifndef PAGE_C
#define PAGE_C

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyDB_FilePage.h"

using namespace std;


MyDB_FilePage::MyDB_FilePage(int pageID, void * address, int size, MyDB_TablePtr table, int index, bool pinned): MyDB_Page(pageID, address, size, pinned)
{
    this->table = table;
    this->tableIndex = index;

    this->loadBytes();
}


void * MyDB_FilePage::getBytes()
{
    return this->pageAddress;
}


void MyDB_FilePage::wroteBytes()
{
    if (! isPinned)
    {
        int fd = open(this->table->getStorageLoc().c_str(), O_CREAT | O_WRONLY | O_FSYNC);
        if( fd == -1 || lseek(fd, this->tableIndex * this->pageSize, SEEK_SET) == -1 || write(fd, this->pageAddress, this->pageSize) == -1 )
        {
            perror("Failed to write to file");
            exit(-1);
        }
        if(close(fd) == -1)
        {
            perror("Failed to close file");
        }
    }
}


void MyDB_FilePage::loadBytes()
{
    int fd = open(this->table->getStorageLoc().c_str(), O_RDONLY | O_FSYNC);
    if( fd == -1 || lseek(fd, this->tableIndex * this->pageSize, SEEK_SET) == -1 || read(fd, this->pageAddress, pageSize) == -1 )
    {
        perror("Failed to read from file");
        exit(-1);
    }
    
    close(fd);
}


#endif


