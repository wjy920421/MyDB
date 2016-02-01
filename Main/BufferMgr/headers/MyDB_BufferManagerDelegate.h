//
//  MyDB_BufferManagerDelegate.h
//  cpp_test
//
//  Created by wjy on 16/1/31.
//  Copyright © 2016年 wjy. All rights reserved.
//

#ifndef BUFFER_MGR_DELEGATE_H
#define BUFFER_MGR_DELEGATE_H

#include <functional>

using namespace std;

typedef function<void(string)> DelegateUnpin;
typedef function<void(string)> DelegateRelease;


#endif /* MyDB_BufferManagerDelegate_h */
