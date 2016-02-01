
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
#include <iostream>

using namespace std;

// Node in LRU Cache
template <class T_KEY, class T_OBJECT>

class MyDB_LRUCacheNode
{
public:
    
    // Constructor
    MyDB_LRUCacheNode(T_KEY key, T_OBJECT const& rObject) : key(key), pObject(&rObject)
    {
        this->pMoreRecentNode = this->pLessRecentNode = nullptr;
    }
    
    // Key to the actual object
    const T_KEY key;
    
    // Pointer to the actual object
    const T_OBJECT * pObject;
    
    // Pointer to the next more recently used node
    MyDB_LRUCacheNode * pMoreRecentNode;
    
    // Pointer to the next less recently used node
    MyDB_LRUCacheNode * pLessRecentNode;
    
};


// LRU Cache
template <class T_KEY, class T_OBJECT>

class MyDB_LRUCache
{
public:
    
    // Constructor
    MyDB_LRUCache(int capacity);
    
    // Destructor
    ~MyDB_LRUCache();
    
    // Accesses and returns the specified object
    T_OBJECT * get(T_KEY key);
    
    // Accesses and return the least recently used object
    T_OBJECT * getLeastRecent();
    
    // Inserts an object
    T_OBJECT * set(T_KEY key, T_OBJECT const& object);
    
    // Removes a specified object
    T_OBJECT * remove(T_KEY key);

    // Removes the least recently used object
    T_OBJECT * removeLeastRecent();
    
    // Returns a specified object without changing the order of nodes
    T_OBJECT * cleanGet(T_KEY key);

    // Returns the size of the LRU
    int size() { return this->map.size(); }

    // Returns the capacity of the LRU
    int getCapacity();

    // Set the capacity of the LRU
    void setCapacity(int capacity);
    
private:
    
    // Capacity of the LRU
    int capacity;
    
    // Hash map from keys to LRU nodes
    unordered_map<T_KEY, MyDB_LRUCacheNode<T_KEY, T_OBJECT> * > map;
    
    // Pointer to the most recently used LRU node
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pMostRecentNode;
    
    // Pointer to the least recent used LRU node
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pLeastRecentNode;
    
    // Inserts an object
    T_OBJECT * internalSet(T_KEY key, T_OBJECT const& object, bool createNode = true);
    
    // Removes a specified object
    T_OBJECT * internalRemove(T_KEY key, bool keepNode = false);
    
};


template <class T_KEY, class T_OBJECT>
MyDB_LRUCache<T_KEY, T_OBJECT>::MyDB_LRUCache(int capacity)
{
    this->capacity = capacity;
    pMostRecentNode = pLeastRecentNode = nullptr;
}


template <class T_KEY, class T_OBJECT>
MyDB_LRUCache<T_KEY, T_OBJECT>::~MyDB_LRUCache()
{
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode = pLeastRecentNode;
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNext;
    while(pNode != nullptr)
    {
        pNext = pNode->pMoreRecentNode;
        this->remove(pNode->key);
        pNode = pNext;
    }
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::get(T_KEY key)
{
    if(this->map.count(key) == 0) return nullptr;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode = this->map[key];
    
    this->internalRemove(key, true);
    this->internalSet(key, *(this->map[key]->pObject), false);
    
    return const_cast<T_OBJECT *>(pNode->pObject);
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::set(T_KEY key, T_OBJECT const& object)
{
    return this->internalSet(key, object);
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::getLeastRecent()
{
    if(this->pLeastRecentNode == nullptr) return nullptr;
    
    return this->get(this->pLeastRecentNode->key);
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::remove(T_KEY key)
{
    return this->internalRemove(key);
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::removeLeastRecent()
{
    if(this->pLeastRecentNode == nullptr) return nullptr;

    return this->internalRemove(this->pLeastRecentNode->key);
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::internalSet(T_KEY key, T_OBJECT const& object, bool createNode)
{
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode;
    
    if(! createNode)
    {
        pNode = this->map[key];
    }
    else
    {
        if(this->map.count(key) > 0) this->remove(key);
        pNode = new MyDB_LRUCacheNode<T_KEY, T_OBJECT>(key, object);
        this->map[key] = pNode;
    }
    
    if(this->pMostRecentNode == nullptr)
    {
        this->pMostRecentNode = this->pLeastRecentNode = pNode;
        pNode->pMoreRecentNode = pNode->pLessRecentNode = nullptr;
    }
    else
    {
        this->pMostRecentNode->pMoreRecentNode = pNode;
        pNode->pLessRecentNode = this->pMostRecentNode;
        pNode->pMoreRecentNode = nullptr;
        this->pMostRecentNode = pNode;
    }
    
    if(map.size() > capacity)
    {
        T_OBJECT * removedObject = const_cast<T_OBJECT *>(this->pLeastRecentNode->pObject);
        this->remove(this->pLeastRecentNode->key);
        
        return removedObject;
    }

    return nullptr;
}

template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::internalRemove(T_KEY key, bool keepNode)
{
    if(this->map.count(key) == 0) return nullptr;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode = this->map[key];
    T_OBJECT * removedObject = const_cast<T_OBJECT *>(pNode->pObject);
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pMoreRecentNode = pNode->pMoreRecentNode;
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pLessRecentNode = pNode->pLessRecentNode;
    
    if(pMoreRecentNode != nullptr) pMoreRecentNode->pLessRecentNode = pLessRecentNode;
    
    if(pLessRecentNode != nullptr) pLessRecentNode->pMoreRecentNode = pMoreRecentNode;
    
    if(pNode == pMostRecentNode) pMostRecentNode = pLessRecentNode;
    
    if(pNode == pLeastRecentNode) pLeastRecentNode = pMoreRecentNode;
    
    if(! keepNode)
    {
        this->map.erase(key);
        delete pNode;
    }

    return removedObject;
}


template <class T_KEY, class T_OBJECT>
int MyDB_LRUCache<T_KEY, T_OBJECT>::getCapacity()
{
    return this->capacity;
}


template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::setCapacity(int capacity)
{
    this->capacity = capacity;

    if(this->pLeastRecentNode != nullptr) while(this->map.size() > capacity) this->remove(this->pLeastRecentNode->key);
}


#endif


