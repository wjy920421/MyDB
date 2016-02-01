
#ifndef BUFFER_MGR_DELEGATE_H
#define BUFFER_MGR_DELEGATE_H

#include <functional>

using namespace std;

typedef function<void(string)> DelegateUnpin;
typedef function<void(string)> DelegateRelease;


#endif /* MyDB_BufferManagerDelegate_h */
