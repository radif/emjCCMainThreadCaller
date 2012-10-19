emjCCMainThreadCaller
=====================

A cross platform c++11 API for the cocos2d-x game engine. It allows you to call back on main thread when using c++11 portable thread library.

you can use it with std::async, std::thread or any other native thread implementation.

Useful for working with threads callbacks


     //you need to save the future outside the stack context to let the tread run trully in background;

     myFutureCollection * fc=new FutureCollection;
     fc->addFuture(std::async(std::launch::async, [](){
     std::cout << "Background Thread ID:"<<std::this_thread::get_id()<<std::endl;

     emjCCPlatformSupport::call_on_main_thread([](){
         std::cout << "Main Thread ID:"<<std::this_thread::get_id()<<std::endl;
     });

     }));
