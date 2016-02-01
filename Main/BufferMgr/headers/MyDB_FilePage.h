
#ifndef FILE_PAGE_H
#define FILE_PAGE_H

#include <string>
#include "MyDB_Page.h"

using namespace std;

class MyDB_FilePage : public MyDB_Page
{
public:

    // Constructor
    MyDB_FilePage(void * address, int size, MyDB_TablePtr table, long index, bool pinned = false);
    
    // Destructor
    ~MyDB_FilePage();
    
    // Write the page to its mapped file
    void writeToFile();

    // Load data from the mapped file
    void loadFromFile();
    
    // Decrease the reference counter by one
    // Called when a page handler is destructed
    void release();

    // Generate page ID according to table name and index
    static string generatePageID(MyDB_TablePtr table, int index);
    
private:

    // Pointer to the table
    MyDB_TablePtr table;

    // Index of the table
    long tableIndex;
};

#endif