
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include "MyDB_Table.h"

using namespace std;

class MyDB_Page
{
public:

    MyDB_Page(void * address, int size, bool pinned = false);

    void * getBytes();
    
    void wroteBytes();

    void loadBytes();

    void retain() { this->referenceCounter ++; }

    void release() { this->referenceCounter --; }

    bool isPinned() { return this->pinned; }

    int getPageID();

protected:

    void * pageAddress;

    int pageSize;

    bool pinned;

    int referenceCounter;

    int pageID;

    static int pageIDCounter;
};

#endif