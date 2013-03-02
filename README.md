emjCCMainThreadCaller
=====================

A cross platform c++11 API for the cocos2d-x game engine. It allows you to call back on main thread when using c++11 standard concurrency library.

you can use it with std::async, std::thread or any other native thread implementation.

Useful for working with threads callbacks


     //you need to save the future outside the stack context to let the tread run trully in background; if you don't do this, the operation will be synchronous;
     struct FutureHolder {std::future<void> future;};

     FutureHolder * f=new FutureHolder;
     f->addFuture(std::async(std::launch::async, [=](){
     std::cout << "Hello from working thread with Thread ID:"<<std::this_thread::get_id()<<std::endl;

     emjCCPlatformSupport::call_on_main_thread([=](){
         std::cout << "Hello from the Main Thread with ID:"<<std::this_thread::get_id()<<std::endl;
         delete f; //deleting the future and holder. If you fail to delete you will have a leak
     });

     }));
