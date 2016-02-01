
#ifndef FILE_PAGE_H
#define FILE_PAGE_H

#include <string>
#include "MyDB_Page.h"

using namespace std;

class MyDB_FilePage : public MyDB_Page
{
public:

    MyDB_FilePage(void * address, int size, MyDB_TablePtr table, long index, bool pinned = false);
    
    ~MyDB_FilePage();
    
    void writeToFile();

    void loadFromFile();
    
    void release();

    static string generatePageID(MyDB_TablePtr table, int index);
    
private:

    MyDB_TablePtr table;

    long tableIndex;
};

#endif