
#ifndef BUFFER_MGR_DELEGATE_H
#define BUFFER_MGR_DELEGATE_H

#include <functional>

using namespace std;

typedef function<void(string)> BufferManagerDelegateUnpin;
typedef function<void(string)> BufferManagerDelegateRelease;

class BufferManagerDelegate
{
public:

    // Delegate function to unpin a specified page
    BufferManagerDelegateUnpin unpin;
    
    // Delegate function to lease a specified page
    BufferManagerDelegateRelease release;
};


#endif
