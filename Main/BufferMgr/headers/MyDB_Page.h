
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include "MyDB_Table.h"

using namespace std;

class MyDB_Page
{
public:

    MyDB_Page(void * address, int size, MyDB_TablePtr table, int index, bool pinned = false);

    void * getBytes();
    
    void wroteBytes();

    void loadBytes();

    void retain() { this->referenceCounter ++; }

    void release() { this->referenceCounter --; }

private:

    void * pageAddress;

    int pageSize;

    bool isPinned;

    MyDB_TablePtr table;

    int tableIndex;

    int referenceCounter;
};

#endif