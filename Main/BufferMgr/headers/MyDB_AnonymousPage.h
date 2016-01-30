
#ifndef PAGE_H
#define PAGE_H

#include <memory>
#include <string>
#include "MyDB_Table.h"

using namespace std;

class MyDB_AnonymousPage : public MyDB_Page
{
public:

    MyDB_AnonymousPage(int pageID, void * address, int size, string tempFile, bool pinned = false);

private:

	string tempFile;
   
};

#endif