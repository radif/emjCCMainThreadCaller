//
//  emjCCMainThreadCaller.h
//  Cocos2d-x
//
//  Created by Radif Sharafullin on 10/16/12.
//
//

#ifndef __emjCCMainThreadCaller__
#define __emjCCMainThreadCaller__

#include <iostream>

/*
 To be used with std::async
 useful for working threads callbacks
 
 myFutureCollection * fc=new FutureCollection;
 
 you need to save the future outside the stack context to let the tread run trully in background
 
 fc->addFuture(std::async(std::launch::async, [](){
 
     std::cout << "Background Thread ID:"<<std::this_thread::get_id()<<std::endl;
 
     emjCCPlatformSupport::call_on_main_thread([](){
         std::cout << "Main Thread ID:"<<std::this_thread::get_id()<<std::endl;
     });
 }));
 
 */

namespace cocos2d{
    void call_on_main_thread(std::function<void()> mainThreadFunction, bool waitUntilDone=false);
}

#endif /* defined(__emjCCMainThreadCaller__) */
