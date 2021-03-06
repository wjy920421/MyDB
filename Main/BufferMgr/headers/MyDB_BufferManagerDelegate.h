
#ifndef BUFFER_MGR_DELEGATE_H
#define BUFFER_MGR_DELEGATE_H

#include <functional>

using namespace std;

class MyDB_Page;

typedef function<void(string)> BufferManagerDelegateUnpin;
typedef function<void(string)> BufferManagerDelegateRelease;
typedef function<void(MyDB_Page *)> BufferManagerDelegateReload;

class BufferManagerDelegate
{
public:

    // Delegate function to unpin a specified page
    BufferManagerDelegateUnpin unpin;
    
    // Delegate function to lease a specified page
    BufferManagerDelegateRelease release;

    // Delegate function to reload an evicted page
    BufferManagerDelegateReload reload;
};


#endif
