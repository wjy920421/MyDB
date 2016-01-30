
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <string>
#include "MyDB_Table.h"

using namespace std;

class MyDB_AnonymousPage : MyDB_Page
{
public:

    MyDB_AnonymousPage(void * address, int size, string tempFile, bool pinned = false);

private:

	string writeBackFile;
   
};

#endif