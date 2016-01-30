
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include "MyDB_Table.h"

using namespace std;

class MyDB_FilePage : MyDB_Page
{
public:

    MyDB_FilePage(void * address, int size, MyDB_TablePtr table, int index, bool pinned = false);

private:

    MyDB_TablePtr table;

    int tableIndex;
};

#endif