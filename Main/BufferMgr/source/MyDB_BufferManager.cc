
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <iostream>
#include <string>

using namespace std;


MyDB_BufferManager::MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile)
{
    this->pageSize = pageSize;
    this->numPages = numPages;

    // Allocate memory blocks
    this->bufferPool = (void **) malloc(sizeof(void *) * numPages);
    int i;
    for(i = 0; i < numPages; i++)
        this->bufferPool[i] = malloc(sizeof(char) * pageSize);

    pLRUCache = new MyDB_LRUCache<string, MyDB_Page>(numPages);
    if (pLRUCache->get("hello") != nullptr) cout << "not null" << endl;
    else cout << "is null" << endl;
}

MyDB_BufferManager::~MyDB_BufferManager ()
{
    delete pLRUCache;

    int i = 0;
    for(i = 0; i < numPages; i++)
        free(this->bufferPool[i]);
    free(this->bufferPool);
}


MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr, long) {
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr, long) {
	return nullptr;		
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
	return nullptr;		
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
}
	
#endif


