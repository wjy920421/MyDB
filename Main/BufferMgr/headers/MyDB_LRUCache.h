
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
#include <iostream>

using namespace std;


template <class T_KEY, class T_OBJECT>

class MyDB_LRUCacheNode
{
public:
    
    MyDB_LRUCacheNode(T_KEY const& rKey, T_OBJECT const& rObject) : pKey(&rKey), pObject(&rObject)
    {
        this->pMoreRecentNode = this->pLessRecentNode = nullptr;
        //cout << "LRUCacheNode created" << endl;
    }
    
    ~MyDB_LRUCacheNode() { /*cout << "LRUCacheNode deleted" << endl;*/ }
    
    const T_KEY * pKey;
    
    const T_OBJECT * pObject;
    
    MyDB_LRUCacheNode * pMoreRecentNode;
    
    MyDB_LRUCacheNode * pLessRecentNode;
    
};


template <class T_KEY, class T_OBJECT>

class MyDB_LRUCache
{
public:
    
    MyDB_LRUCache(int capacity) { this->capacity = capacity; pMostRecentNode = pLeastRecentNode = nullptr; }
    
    ~MyDB_LRUCache();
    
    T_OBJECT * get(T_KEY const& key);
    
    T_OBJECT * getLeastUsed();
    
    void set(T_KEY const& key, T_OBJECT const& object);
    
    void remove(T_KEY const& key);

    int getCapacity();

    void setCapacity(int capacity);
    
private:
    
    int capacity;
    
    unordered_map<T_KEY, MyDB_LRUCacheNode<T_KEY, T_OBJECT> * > map;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pMostRecentNode;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pLeastRecentNode;
    
    void internalSet(T_KEY const& key, T_OBJECT const& object, bool createNode = true);
    
    void internalRemove(T_KEY const& key, bool keepNode = false);
    
    //void printNodes();
    
};


template <class T_KEY, class T_OBJECT>
MyDB_LRUCache<T_KEY, T_OBJECT>::~MyDB_LRUCache()
{
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode = pLeastRecentNode;
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNext;
    //cout << "size of map: " << map.size() << endl;
    while(pNode != nullptr)
    {
        pNext = pNode->pMoreRecentNode;
        this->remove(*(pNode->pKey));
        pNode = pNext;
        //cout << "size of map: " << map.size() << endl;
    }
    //cout << "size of map: " << map.size() << endl;
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::get(T_KEY const& key)
{
    if(this->map.count(key) == 0) return nullptr;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode = this->map[key];
    
    this->internalRemove(key, true);
    this->internalSet(key, *(this->map[key]->pObject), false);
    
    return const_cast<T_OBJECT *>(pNode->pObject);
}


template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::set(T_KEY const& key, T_OBJECT const& object)
{
    this->internalSet(key, object);
}


template <class T_KEY, class T_OBJECT>
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::getLeastUsed()
{
    if(this->pLeastRecentNode == nullptr) return nullptr;
    
    return this->get(*(this->pLeastRecentNode->pKey));
}


template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::remove(T_KEY const& key)
{
    this->internalRemove(key);
}


template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::internalSet(T_KEY const& key, T_OBJECT const& object, bool createNode)
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
        this->remove(*(this->pLeastRecentNode->pKey));
    }
    
    //this->printNodes();
}

template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::internalRemove(T_KEY const& key, bool keepNode)
{
    if(this->map.count(key) == 0) return;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pNode = this->map[key];
    
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
}


template <class T_KEY, class T_OBJECT>
int MyDB_LRUCache<T_KEY, T_OBJECT>::getCapacity()
{
    return this->capacity;
}


template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::setCapacity(int capacity)
{
    int prev_capacity = this->capacity;
    this->capacity = capacity;

    while(prev_capacity-- > capacity) this->remove(*(this->pLeastRecentNode->pKey));
}


/*
template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::printNodes()
{
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * p = pMostRecentNode;
    while(p != nullptr)
    {
        cout << "key: " << *(p->pKey) << "; ";
        p = p->pLessRecentNode;
    }
    cout << "most recently used: " << "key: " << *(pMostRecentNode->pKey) << "; ";
    cout << "least recently used: " << "key: " << *(pLeastRecentNode->pKey) << "; ";
    cout << endl;
    
    cout << "size: " << this->map.size() << endl;
}
*/

#endif


