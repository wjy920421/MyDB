
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include <iostream>
#include <string>

using namespace std;


MyDB_BufferManager::MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile)
{
    pLRUCache = new MyDB_LRUCache<string, MyDB_Page>(numPages);
    if (pLRUCache->get("hello") != nullptr) cout << "not null" << endl;
    else cout << "is null" << endl;
}

MyDB_BufferManager::~MyDB_BufferManager ()
{
    delete pLRUCache;
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


