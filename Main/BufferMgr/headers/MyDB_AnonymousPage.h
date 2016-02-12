
#ifndef ANONYMOUS_PAGE_H
#define ANONYMOUS_PAGE_H

#include <string>
#include "MyDB_Page.h"

using namespace std;

class MyDB_AnonymousPage : public MyDB_Page
{
public:

    // Constructor
    MyDB_AnonymousPage(void * address, int size, string tempFile, bool pinned = false);
    
    // Destructor
    ~MyDB_AnonymousPage();
    
    // Write the page to the temporary file
    void writeToFile();
    
    // Load the page from the temporary file
    void loadFromFile();

    // Decrease the reference counter by one
    // Called when a page handler is destructed
    void release();

private:

    // Temporary file to which the anonymous page is mapped
	string tempFile;
    
    // Offset in the temporay file where the page is actually stored
    int fileOffset;
};

#endif