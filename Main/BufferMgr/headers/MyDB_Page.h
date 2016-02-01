
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

    // Constructor
    MyDB_Page(void * address, int size, bool pinned = false);

    // Virtual destructor
    virtual ~MyDB_Page() {}

    // Returns the page's address in buffer
    void * getBytes();
    
    // Marks the page as dirty
    void wroteBytes() { this->dirty = true; }
    
    // Write the page to the mapped file
    virtual void writeToFile() {}

    // Load data from the mapped file
    virtual void loadFromFile() {}

    // Increase the reference counter by one
    // Called when a page handler is constructed
    virtual void retain() { this->referenceCounter ++; }

    // Decrease the reference counter by one
    // Called when a page handler is destructed
    virtual void release() { this->referenceCounter --; }

    // Pins the page
    void setPinned(bool pinned) { this->pinned = pinned; }

    // Check if the page is pinned
    bool isPinned() { return this->pinned; }

    // Returns page ID
    string getPageID();

    // Set the address in buffer
    void setBuffer(void * address) { this->pageAddress = address; }

    // Returns the page's address in buffer
    void * getBuffer() { return this->getBytes(); }
    
    // Set delegate
    void setDelegate(BufferManagerDelegate delegate) { this->bufferManagerDelegate = delegate; }
    
    // Evict the page out of buffer
    void evict() { this->evicted = true; }

protected:

    // Address in buffer
    void * pageAddress;

    // Size of the page
    int pageSize;

    // Whether the page is pinned
    bool pinned;
    
    // Whether the page is dirty
    bool dirty;
    
    // Whether the page is evicted out of the buffer
    bool evicted;

    // Number of referenced page handlers
    int referenceCounter;

    // Page ID
    string pageID;

    // Static variable for generating page ID
    static int pageIDCounter;
    
    // Delegate
    BufferManagerDelegate bufferManagerDelegate;
    
};

#endif