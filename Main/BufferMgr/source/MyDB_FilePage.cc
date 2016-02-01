
#ifndef FILE_PAGE_C
#define FILE_PAGE_C

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "MyDB_FilePage.h"

using namespace std;


MyDB_FilePage::MyDB_FilePage(void * address, int size, MyDB_TablePtr table, long index, bool pinned): MyDB_Page(address, size, pinned)
{
    this->pageID = MyDB_FilePage::generatePageID(table, index);
    this->table = table;
    this->tableIndex = index;
    this->loadFromFile();
}


MyDB_FilePage::~MyDB_FilePage()
{

}


void MyDB_FilePage::release()
{
    MyDB_Page::release();
    
    if(this->referenceCounter == 0)
    {
        if(this->evicted)
        {
            this->delegateRelease(this->pageID);
            delete this;
        }
        else if(this->isPinned())
        {
            this->delegateUnpin(this->pageID);
        }
    }
}


string MyDB_FilePage::generatePageID(MyDB_TablePtr table, int index)
{
    return table->getName() + "_" + to_string(index);
}


void MyDB_FilePage::writeToFile()
{
    if (!this->isPinned() && this->dirty)
    {
        int fd = open(this->table->getStorageLoc().c_str(), O_CREAT | O_RDWR | O_FSYNC, S_IRWXU );
        if( fd == -1 || lseek(fd, this->tableIndex * this->pageSize, SEEK_SET) == -1 || write(fd, this->pageAddress, this->pageSize) == -1 )
        {
            fprintf(stderr, "Failed to write page \'%s\' to \'%s\'", this->pageID.c_str(), this->table->getStorageLoc().c_str());
            exit(-1);
        }
        if(close(fd) == -1)
        {
            fprintf(stderr, "Failed to close \'%s\'", this->table->getStorageLoc().c_str());
        }
    }
}


void MyDB_FilePage::loadFromFile()
{
    int fd = open(this->table->getStorageLoc().c_str(), O_CREAT | O_RDWR | O_FSYNC, S_IRWXU );
    if( fd == -1 )
    {
        fprintf(stderr, "Failed to open file");
    }
    else if ( lseek(fd, this->tableIndex * this->pageSize, SEEK_SET) == -1 )
    {
        fprintf(stderr, "Failed to use lseek");
    }
    else if ( read(fd, this->pageAddress, pageSize) == -1 )
    {
        fprintf(stderr, "Failed to read from file");
    }
    
    close(fd);
}


#endif


