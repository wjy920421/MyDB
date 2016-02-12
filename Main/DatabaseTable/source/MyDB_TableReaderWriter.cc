
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <sstream>
#include "MyDB_PageHandle.h"
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"
#include "MyDB_TableRecordIterator.h"

using namespace std;


MyDB_TableReaderWriter::MyDB_TableReaderWriter (MyDB_TablePtr tablePtr, MyDB_BufferManagerPtr myBuffer)
{
    this->tablePtr = tablePtr;
    this->bufferManagerPtr = myBuffer;
    
    if (this->tablePtr->lastPage() >= 0)
    {
        for (int i = 0; i <= this->tablePtr->lastPage(); i++)
        {
            MyDB_PageHandle pageHandle = this->bufferManagerPtr->getPage(this->tablePtr, i);
            this->pageVector.push_back(MyDB_PageReaderWriter(pageHandle));
        }
    }
    else
    {
        MyDB_PageHandle pageHandle = this->bufferManagerPtr->getPage(this->tablePtr, 0);
        this->pageVector.push_back(MyDB_PageReaderWriter(pageHandle, true));
    }
}


MyDB_PageReaderWriter & MyDB_TableReaderWriter::operator [] (size_t i)
{
    return this->pageVector[i];
}


MyDB_RecordPtr MyDB_TableReaderWriter::getEmptyRecord ()
{
	return make_shared <MyDB_Record> (this->tablePtr->getSchema());
}


MyDB_PageReaderWriter & MyDB_TableReaderWriter::last ()
{
    return this->pageVector.back();
}


void MyDB_TableReaderWriter::append (MyDB_RecordPtr recordPtr)
{
    if (! this->last().append(recordPtr))
    {
        MyDB_PageHandle pageHandle = this->bufferManagerPtr->getPage(this->tablePtr, this->tablePtr->lastPage() + 1);
        this->tablePtr->setLastPage(this->tablePtr->lastPage() + 1);
        MyDB_PageReaderWriter newPageReaderWriter(pageHandle, true);
        newPageReaderWriter.append(recordPtr);
        this->pageVector.push_back(newPageReaderWriter);
    }
}


void MyDB_TableReaderWriter::loadFromTextFile (string filename)
{
    this->pageVector.clear();
    MyDB_PageHandle pageHandle = this->bufferManagerPtr->getPage(this->tablePtr, 0);
    this->pageVector.push_back(MyDB_PageReaderWriter(pageHandle, true));

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        MyDB_RecordPtr recordPtr = this->getEmptyRecord();
        while (file.good())
        {
            getline(file,line);
            recordPtr->fromString(line);
            this->append(recordPtr);
        }
        file.close();
    }
    else 
    {  
        fprintf(stderr, "Failed to open \'%s\'", filename.c_str()); 
    }
}


MyDB_RecordIteratorPtr MyDB_TableReaderWriter::getIterator (MyDB_RecordPtr recordPtr)
{
	return make_shared <MyDB_TableRecordIterator> (recordPtr, this);
}


void MyDB_TableReaderWriter::writeIntoTextFile (string filename)
{
    ofstream file(filename);
    if (file.is_open())
    {
        ostringstream stream;
        MyDB_RecordPtr recordPtr = this->getEmptyRecord();
        MyDB_RecordIteratorPtr iterator = this->getIterator(recordPtr);
        while (iterator->hasNext())
        {
            iterator->getNext();
            stream << recordPtr;
            file << stream.str() << endl;
        }
        file.close();
    }
    else 
    {  
        fprintf(stderr, "Failed to open \'%s\'", filename.c_str()); 
    }
}


#endif


