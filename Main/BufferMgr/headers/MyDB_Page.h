
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <string>
#include "MyDB_Table.h"
#include "MyDB_BufferManagerDelegate.h"

using namespace std;

class MyDB_Page
{
public:

    MyDB_Page(void * address, int size, bool pinned = false);

    virtual ~MyDB_Page() {}

    void * getBytes();
    
    void wroteBytes() { this->dirty = true; }
    
    virtual void writeToFile() {}

    virtual void retain() { this->referenceCounter ++; }

    virtual void release() { this->referenceCounter --; }

    void setPinned(bool pinned) { this->pinned = pinned; }

    bool isPinned() { return this->pinned; }

    string getPageID();

    void setBuffer(void * address) { this->pageAddress = address; }

    void * getBuffer() { return this->getBytes(); }
    
    void setDelegate(DelegateUnpin delegateUnpin, DelegateRelease delegateRelease) { this->delegateUnpin = delegateUnpin; this->delegateRelease = delegateRelease; }
    
    void evict() { this->evicted = true; }

protected:

    void * pageAddress;

    int pageSize;

    bool pinned;
    
    bool dirty;
    
    bool evicted;

    int referenceCounter;

    string pageID;

    static int pageIDCounter;
    
    DelegateUnpin delegateUnpin;
    
    DelegateRelease delegateRelease;
    
};

#endif