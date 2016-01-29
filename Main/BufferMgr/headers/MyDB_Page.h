
#ifndef PAGE_H
#define PAGE_H

#include <memory>

using namespace std;

class MyDB_Page
{
public:

    MyDB_Page(void * address, int size);

    void * getBytes();
    
    void wroteBytes();

private:

    void * pageAddress;

    int pageSize;

    bool isPinned;

    static int referenceCounter = 0;
};

#endif