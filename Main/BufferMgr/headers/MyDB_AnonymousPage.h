
#ifndef ANONYMOUS_PAGE_H
#define ANONYMOUS_PAGE_H

#include <string>
#include "MyDB_Page.h"

using namespace std;

class MyDB_AnonymousPage : public MyDB_Page
{
public:

    MyDB_AnonymousPage(void * address, int size, string tempFile, bool pinned = false);

    void wroteBytes();

    void loadBytes();

    virtual void release();

private:

	string tempFile;
   
};

#endif