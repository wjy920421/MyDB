
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include "MyDB_Table.h"

using namespace std;

class MyDB_Page
{
public:

    MyDB_Page(int pageID, void * address, int size, MyDB_TablePtr table, int index, bool pinned = false);

    void * getBytes();
    
    void wroteBytes();

    void loadBytes();

    void retain() { this->referenceCounter ++; }

    void release() { this->referenceCounter --; }

private:

    void * pageAddress;

    int pageSize;

    bool isPinned;

    int referenceCounter;
};

#endif