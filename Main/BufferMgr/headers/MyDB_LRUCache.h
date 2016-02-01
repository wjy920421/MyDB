
#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
#include <iostream>

using namespace std;


template <class T_KEY, class T_OBJECT>

class MyDB_LRUCacheNode
{
public:
    
    MyDB_LRUCacheNode(T_KEY key, T_OBJECT const& rObject) : key(key), pObject(&rObject)
    {
        this->pMoreRecentNode = this->pLessRecentNode = nullptr;
        //cout << "LRUCacheNode created" << endl;
    }
    
    ~MyDB_LRUCacheNode() { /*cout << "LRUCacheNode deleted" << endl;*/ }
    
    const T_KEY key;
    
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
    
    T_OBJECT * get(T_KEY key);
    
    T_OBJECT * getLeastUsed();
    
    T_OBJECT * set(T_KEY key, T_OBJECT const& object);
    
    T_OBJECT * remove(T_KEY key);
    
    T_OBJECT * cleanGet(T_KEY key);
    
    T_OBJECT * cleanSet(T_KEY key, T_OBJECT const& object);

    int size() { return this->map.size(); }

    int getCapacity();

    void setCapacity(int capacity);
    
private:
    
    int capacity;
    
    unordered_map<T_KEY, MyDB_LRUCacheNode<T_KEY, T_OBJECT> * > map;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pMostRecentNode;
    
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * pLeastRecentNode;
    
    T_OBJECT * internalSet(T_KEY key, T_OBJECT const& object, bool createNode = true);
    
    T_OBJECT * internalRemove(T_KEY key, bool keepNode = false);
    
    void printNodes();
    
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
        this->remove(pNode->key);
        pNode = pNext;
        //cout << "size of map: " << map.size() << endl;
    }
    //cout << "size of map: " << map.size() << endl;
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
T_OBJECT * MyDB_LRUCache<T_KEY, T_OBJECT>::getLeastUsed()
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
    
    this->printNodes();
    
    if(map.size() > capacity)
    {
        T_OBJECT * removedObject = const_cast<T_OBJECT *>(this->pLeastRecentNode->pObject);
        this->remove(this->pLeastRecentNode->key);
        
        this->printNodes();
        
        return removedObject;
    }
    
    //this->printNodes();

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



template <class T_KEY, class T_OBJECT>
void MyDB_LRUCache<T_KEY, T_OBJECT>::printNodes()
{
    /*
    MyDB_LRUCacheNode<T_KEY, T_OBJECT> * p = pMostRecentNode;
    while(p != nullptr)
    {
        if(p->pObject!=nullptr) cout << "key: " << p->key << ", value: " << "object" << "; ";
        else cout << "key: " << p->key << ", value: " << "null" << "; ";
        p = p->pLessRecentNode;
    }
    cout << "most recently used: " << "key: " << pMostRecentNode->key << "; ";
    cout << "least recently used: " << "key: " << pLeastRecentNode->key << "; ";
    cout << endl;
    
    cout << "size: " << this->map.size() << endl;
     */
}


#endif


