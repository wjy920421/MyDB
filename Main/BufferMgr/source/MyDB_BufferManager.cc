
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include <iostream>
#include <string>
#include "MyDB_BufferManager.h"
#include "MyDB_FilePage.h"
#include "MyDB_AnonymousPage.h"

using namespace std;


MyDB_BufferManager::MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile)
{
    this->pageSize = pageSize;
    this->numPages = numPages;
    this->tempFile = tempFile;
    
    // Allocate memory blocks
    this->bufferPool = (void **) malloc(sizeof(void *) * numPages);
    int i;
    for(i = 0; i < numPages; i++)
    {
        this->bufferPool[i] = malloc(sizeof(char) * pageSize);
        availableBufferPool.push(this->bufferPool[i]);
    }
    
    // Initiate LRU Caches
    pinnedLRUCache = new MyDB_LRUCache<string, MyDB_Page>(0);
    unpinnedLRUCache = new MyDB_LRUCache<string, MyDB_Page>(numPages);
    
    this->bufferManagerDelegate.unpin = bind(&MyDB_BufferManager::doDelegateUnpin, this, placeholders::_1);
    this->bufferManagerDelegate.release = bind(&MyDB_BufferManager::doDelegateRelease, this, placeholders::_1);
    this->bufferManagerDelegate.reload = bind(&MyDB_BufferManager::doDelegateReload, this, placeholders::_1);
}

MyDB_BufferManager::~MyDB_BufferManager ()
{
    MyDB_Page * page;
    
    while ((page = this->pinnedLRUCache->removeLeastRecent()) != nullptr )
    {
        page->writeToFile();
        delete page;
    }
    
    while ((page = this->unpinnedLRUCache->removeLeastRecent()) != nullptr )
    {
        page->writeToFile();
        delete page;
    }
    
    delete pinnedLRUCache;
    delete unpinnedLRUCache;
    
    int i = 0;
    for(i = 0; i < numPages; i++)
        free(this->bufferPool[i]);
    free(this->bufferPool);
}

int MyDB_BufferManager::getPageSize()
{
    return this->pageSize;
}


MyDB_PageHandle MyDB_BufferManager::getPage (MyDB_TablePtr whichTable, long i)
{
    string pageID = MyDB_FilePage::generatePageID(whichTable, i);
    MyDB_Page * page;
    
    if((page = this->unpinnedLRUCache->get(pageID)) != nullptr)
    {
        return make_shared<MyDB_PageHandleBase>(page);
    }
    else if ((page = this->pinnedLRUCache->get(pageID)) != nullptr)
    {
        // getPage returns the existing pinned page without unpinning it
        // this->doDelegateUnpin(pageID);
        return make_shared<MyDB_PageHandleBase>(page);
    }
    else if(this->unpinnedLRUCache->getCapacity() != 0)
    {
        if(this->availableBufferPool.empty())
            this->doDelegateRelease(this->unpinnedLRUCache->getLeastRecent()->getPageID());
        
        page = new MyDB_FilePage(this->availableBufferPool.front(), this->pageSize, whichTable, i);
        page->setDelegate(this->bufferManagerDelegate);
        this->availableBufferPool.pop();
        this->unpinnedLRUCache->set(page->getPageID(), *page);
        return make_shared<MyDB_PageHandleBase>(page);
    }
    
    return nullptr;
}

MyDB_PageHandle MyDB_BufferManager::getPage ()
{
    if(this->unpinnedLRUCache->getCapacity() != 0)
    {
        if(this->availableBufferPool.empty())
            this->doDelegateRelease(this->unpinnedLRUCache->getLeastRecent()->getPageID());
        
        MyDB_Page * page = new MyDB_AnonymousPage(this->availableBufferPool.front(), this->pageSize, this->tempFile);
        page->setDelegate(this->bufferManagerDelegate);
        this->availableBufferPool.pop();
        this->unpinnedLRUCache->set(page->getPageID(), *page);
        return make_shared<MyDB_PageHandleBase>(page);
    }
    
    return nullptr;
}

MyDB_PageHandle MyDB_BufferManager::getPinnedPage (MyDB_TablePtr whichTable, long i)
{
    string pageID = MyDB_FilePage::generatePageID(whichTable, i);
    MyDB_Page * page;
    
    if((page = this->pinnedLRUCache->get(pageID)) != nullptr)
    {
        return make_shared<MyDB_PageHandleBase>(page);
    }
    else if ((page = this->unpinnedLRUCache->get(pageID)) != nullptr)
    {
        ////////////////////////////
        // Pins the existing page //
        this->doDelegatePin(pageID);
        ////////////////////////////
        return make_shared<MyDB_PageHandleBase>(page);
    }
    else if(this->pinnedLRUCache->size() < this->numPages)
    {
        if(this->availableBufferPool.empty())
            this->doDelegateRelease(this->unpinnedLRUCache->getLeastRecent()->getPageID());
        
        this->pinnedLRUCache->setCapacity(this->pinnedLRUCache->getCapacity() + 1);
        this->unpinnedLRUCache->setCapacity(this->unpinnedLRUCache->getCapacity() - 1);
        page = new MyDB_FilePage(this->availableBufferPool.front(), this->pageSize, whichTable, i, true);
        page->setDelegate(this->bufferManagerDelegate);
        this->availableBufferPool.pop();
        this->pinnedLRUCache->set(page->getPageID(), *page);
        return make_shared<MyDB_PageHandleBase>(page);
    }
    
    return nullptr;
}

MyDB_PageHandle MyDB_BufferManager::getPinnedPage ()
{
    if(this->pinnedLRUCache->size() < this->numPages)
    {
        if(this->availableBufferPool.empty())
            this->doDelegateRelease(this->unpinnedLRUCache->getLeastRecent()->getPageID());
        
        this->pinnedLRUCache->setCapacity(this->pinnedLRUCache->getCapacity() + 1);
        this->unpinnedLRUCache->setCapacity(this->unpinnedLRUCache->getCapacity() - 1);
        MyDB_Page * page = new MyDB_AnonymousPage(this->availableBufferPool.front(), this->pageSize, this->tempFile, true);
        page->setDelegate(this->bufferManagerDelegate);
        this->availableBufferPool.pop();
        this->pinnedLRUCache->set(page->getPageID(), *page);
        return make_shared<MyDB_PageHandleBase>(page);
    }
    
    return nullptr;
}


void MyDB_BufferManager::unpin (MyDB_PageHandle unpinMe)
{
    this->doDelegateUnpin(unpinMe->getPageID());
}


void MyDB_BufferManager::pin (MyDB_PageHandle pinMe)
{
    this->doDelegatePin(pinMe->getPageID());
}


void MyDB_BufferManager::doDelegateUnpin(string pageID)
{
    MyDB_Page * unpinnedPage = this->pinnedLRUCache->remove(pageID);
    if(unpinnedPage != nullptr)
    {
        unpinnedPage->setPinned(false);
        this->pinnedLRUCache->setCapacity(this->pinnedLRUCache->getCapacity() - 1);
        this->unpinnedLRUCache->setCapacity(this->unpinnedLRUCache->getCapacity() + 1);
        this->unpinnedLRUCache->set(unpinnedPage->getPageID(), *unpinnedPage);
    }
}


void MyDB_BufferManager::doDelegatePin(string pageID)
{
    MyDB_Page * pinnedPage = this->unpinnedLRUCache->remove(pageID);
    if(pinnedPage != nullptr)
    {
        pinnedPage->setPinned(true);
        this->pinnedLRUCache->setCapacity(this->pinnedLRUCache->getCapacity() + 1);
        this->unpinnedLRUCache->setCapacity(this->unpinnedLRUCache->getCapacity() - 1);
        this->pinnedLRUCache->set(pinnedPage->getPageID(), *pinnedPage);
    }
}


void MyDB_BufferManager::doDelegateRelease(string pageID)
{
    MyDB_Page * releasedPage;
    
    if((releasedPage = this->pinnedLRUCache->remove(pageID)) != nullptr)
    {
        this->pinnedLRUCache->setCapacity(this->pinnedLRUCache->getCapacity() - 1);
        this->unpinnedLRUCache->setCapacity(this->unpinnedLRUCache->getCapacity() + 1);
        this->availableBufferPool.push(releasedPage->getBuffer());
    }
    else if((releasedPage = this->unpinnedLRUCache->remove(pageID)) != nullptr)
    {
        this->availableBufferPool.push(releasedPage->getBuffer());
    }
    else return;
    
    releasedPage->writeToFile();
    releasedPage->evict();
}


void MyDB_BufferManager::doDelegateReload(MyDB_Page * reloadPage)
{
    string pageID = reloadPage->getPageID();
    MyDB_Page * existingPage;
    
    if((existingPage = this->pinnedLRUCache->get(pageID)) != nullptr)
    {
        existingPage->loadFromFile();
    }
    else if((existingPage = this->unpinnedLRUCache->get(pageID)) != nullptr)
    {
        existingPage->loadFromFile();
    }
    else if(this->unpinnedLRUCache->getCapacity() != 0)
    {
        if(this->availableBufferPool.empty())
            this->doDelegateRelease(this->unpinnedLRUCache->getLeastRecent()->getPageID());
        
        reloadPage->setBuffer(this->availableBufferPool.front());
        this->availableBufferPool.pop();
        this->unpinnedLRUCache->set(reloadPage->getPageID(), *reloadPage);
        reloadPage->reload();
    }
}


#endif


