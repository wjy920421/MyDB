
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
    
    virtual void wroteBytes();

    virtual void loadBytes();

    virtual void retain() { this->referenceCounter ++; }

    virtual void release() { this->referenceCounter --; }

    void setPinned(bool pinned) { this->pinned = pinned; }

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