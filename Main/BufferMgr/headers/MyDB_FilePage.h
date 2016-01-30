
#ifndef FILE_PAGE_H
#define FILE_PAGE_H

#include "MyDB_Page.h"

using namespace std;

class MyDB_FilePage : public MyDB_Page
{
public:

    MyDB_FilePage(void * address, int size, MyDB_TablePtr table, int index, bool pinned = false);

    void wroteBytes();

    void loadBytes();
    
private:

    MyDB_TablePtr table;

    int tableIndex;
};

#endif