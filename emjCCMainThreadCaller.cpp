//
//  emjCCMainThreadCaller.cpp
//  Cocos2d-x
//
//  Created by Radif Sharafullin on 10/16/12.
//
//

#include "emjCCMainThreadCaller.h"
#include "cocos2d.h"
#include <future>

namespace cocos2d{
    
    class emjCCMainThreadCaller;
    
    static emjCCMainThreadCaller * _sharedInstance=0x00;
    
    class emjCCMainThreadCaller : public cocos2d::CCObject {
        std::vector<std::function<void()>> _functions;
        std::mutex _functionsMutex;
        
        void flush(){
            decltype(_functions) functionsCopy;
            {
                std::lock_guard<std::mutex> lock(_functionsMutex);
                functionsCopy=std::move(_functions);
                using cocos2d::SEL_SCHEDULE;
                cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(emjCCMainThreadCaller::flush), this);
                _functions.erase(_functions.begin(), _functions.end());
                assert(_functions.size()==0);
            }
            //call the functions outside the mutext lock
            //functionsCopy is always on main thread, so no data races
            for (auto i=functionsCopy.begin(); i!=functionsCopy.end(); ++i)
                (*i)();
        }
        
    public:
        static emjCCMainThreadCaller * sharedInstance(){
            static std::once_flag onceFlag;
            std::call_once(onceFlag,[](){
                if (!_sharedInstance)
                    _sharedInstance=new emjCCMainThreadCaller();
            });
            return _sharedInstance;
        }
        void call_on_main_thread(std::function<void()> && mainThreadFunction, bool waitUntilDone=false){
            {
            std::lock_guard<std::mutex> lock(_functionsMutex);
            _functions.push_back(std::move(mainThreadFunction));
            using cocos2d::SEL_SCHEDULE;
            cocos2d::CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(emjCCMainThreadCaller::flush), this);
            cocos2d::CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(emjCCMainThreadCaller::flush), this, 0, false);
            }
            
            if (waitUntilDone)
                std::this_thread::sleep_for(std::chrono::microseconds(50));// or use this?: std::this_thread::yield();
                //std::this_thread::yield();
        }
        
        
    };
    
    
    void call_on_main_thread(std::function<void()> mainThreadFunction, bool waitUntilDone){
        emjCCMainThreadCaller::sharedInstance()->call_on_main_thread(std::move(mainThreadFunction), waitUntilDone);
    }
}

