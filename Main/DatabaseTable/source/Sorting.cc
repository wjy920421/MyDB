
#ifndef SORT_C
#define SORT_C

#include <queue>
#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"
#include "MyDB_TableRecIteratorAlt.h"
#include "MyDB_TableReaderWriter.h"
#include "IteratorComparator.h"
#include "Sorting.h"

using namespace std;

void mergeIntoFile (MyDB_TableReaderWriter &sortIntoMe, vector <MyDB_RecordIteratorAltPtr> &mergeUs, 
    function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs)
{

    // create the comparator and the priority queue
    IteratorComparator temp (comparator, lhs, rhs);
    priority_queue <MyDB_RecordIteratorAltPtr, vector <MyDB_RecordIteratorAltPtr>, IteratorComparator> pq (temp);

    // load up the set
    for (MyDB_RecordIteratorAltPtr m : mergeUs) {
        if (m->advance ()) {
            pq.push (m);
        }
    }

    // and write everyone out
    int counter = 0;
    while (pq.size () != 0) {

        // write the dude to the output
        auto myIter = pq.top ();
        myIter->getCurrent (lhs);
        sortIntoMe.append (lhs);
        counter++;

        // remove from the q
        pq.pop ();

        // re-insert
        if (myIter->advance ()) {
            pq.push (myIter);
        }
    }
}

void appendRecord (MyDB_PageReaderWriter &curPage, vector <MyDB_PageReaderWriter> &returnVal, 
    MyDB_RecordPtr appendMe, MyDB_BufferManagerPtr parent)
{

    // try to append to the current page
    if (!curPage.append (appendMe)) {

        // if we cannot, then add a new one to the output vector
        returnVal.push_back (curPage);
        MyDB_PageHandle pageHandle = parent->getPage();
        MyDB_PageReaderWriter temp (parent, pageHandle, true);
        temp.append (appendMe);
        curPage = temp;
    }
}

vector <MyDB_PageReaderWriter> mergeIntoList (MyDB_BufferManagerPtr parent, MyDB_RecordIteratorAltPtr leftIter, 
    MyDB_RecordIteratorAltPtr rightIter, function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs)
{
    
    vector <MyDB_PageReaderWriter> returnVal;
    MyDB_PageHandle pageHandle = parent->getPage();
    MyDB_PageReaderWriter curPage (parent, pageHandle, true);
    bool lhsLoaded = false, rhsLoaded = false;

    // if one of the runs is empty, get outta here
    if (!leftIter->advance ()) {
        while (rightIter->advance ()) {
            rightIter->getCurrent (rhs);
            appendRecord (curPage, returnVal, rhs, parent);
        }
    } else if (!rightIter->advance ()) {
        while (leftIter->advance ()) {
            leftIter->getCurrent (lhs);
            appendRecord (curPage, returnVal, lhs, parent);
        }
    } else {
        while (true) {
    
            // get the two records

            // here's a bit of an optimization... if one of the records is loaded, don't re-load
            if (!lhsLoaded) {
                leftIter->getCurrent (lhs);
                lhsLoaded = true;
            }

            if (!rhsLoaded) {
                rightIter->getCurrent (rhs);        
                rhsLoaded = true;
            }
    
            // see if the lhs is less
            if (comparator ()) {
                appendRecord (curPage, returnVal, lhs, parent);
                lhsLoaded = false;

                // deal with the case where we have to append all of the right records to the output
                if (!leftIter->advance ()) {
                    appendRecord (curPage, returnVal, rhs, parent);
                    while (rightIter->advance ()) {
                        rightIter->getCurrent (rhs);
                        appendRecord (curPage, returnVal, rhs, parent);
                    }
                    break;
                }
            } else {
                appendRecord (curPage, returnVal, rhs, parent);
                rhsLoaded = false;

                // deal with the ase where we have to append all of the right records to the output
                if (!rightIter->advance ()) {
                    appendRecord (curPage, returnVal, lhs, parent);
                    while (leftIter->advance ()) {
                        leftIter->getCurrent (lhs);
                        appendRecord (curPage, returnVal, lhs, parent);
                    }
                    break;
                }
            }
        }
    }
    
    // remember the current page
    returnVal.push_back (curPage);
    
    // outta here!
    return returnVal;
}

vector <MyDB_PageReaderWriter> mergePages (MyDB_BufferManagerPtr bufferMgr, vector <MyDB_PageReaderWriter> & pages, int lowPage, int highPage,
    function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs)
{
    //if (lowPage > highPage)
    //{
    //    return vector <MyDB_PageReaderWriter> ();
    //}
    if (lowPage == highPage)
    {
        vector <MyDB_PageReaderWriter> returnVal;
        returnVal.push_back(pages[lowPage]);
        return returnVal;
    }
    else if (lowPage + 1 == highPage)
    {
        return mergeIntoList(bufferMgr, pages[lowPage].getIteratorAlt(), pages[highPage].getIteratorAlt(), comparator, lhs, rhs);
    }
    else
    {
        int midPage = (lowPage + highPage) / 2;
        //cout << lowPage << " " << midPage << " " << highPage << endl;
        vector <MyDB_PageReaderWriter> lowSorted = mergePages(bufferMgr, pages, lowPage, midPage, comparator, lhs, rhs);
        vector <MyDB_PageReaderWriter> highSorted = mergePages(bufferMgr, pages, midPage + 1, highPage, comparator, lhs, rhs);
        return mergeIntoList(bufferMgr, getIteratorAlt(lowSorted), getIteratorAlt(highSorted), comparator, lhs, rhs);
    }
}
    
void sort (int runSize, MyDB_TableReaderWriter & tableIn, MyDB_TableReaderWriter & tableOut, 
    function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs)
{
    int numPages = tableIn.getNumPages();
    int pagesAccessed = 0;
    vector <MyDB_RecordIteratorAltPtr> bufferIters;

    while (pagesAccessed < numPages)
    {
        vector <MyDB_PageReaderWriter> pages;
        for (int i = 0; i < runSize; i++)
        {
            if (pagesAccessed < numPages) pages.push_back(*(tableIn[pagesAccessed++].sort(comparator, lhs, rhs)));
            else break;
        }

        vector <MyDB_PageReaderWriter> sortedBuffer = mergePages(tableIn.getBufferMgr(), pages, 0, pages.size() - 1, comparator, lhs, rhs);
        bufferIters.push_back(getIteratorAlt(sortedBuffer));
    }
    
    mergeIntoFile (tableOut, bufferIters, comparator, lhs, rhs);
}

#endif
