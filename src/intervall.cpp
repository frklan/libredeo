#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include "timer.h"
#include "intervall.h"



namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  IntervallTimer::IntervallTimer()
  { 
    std::cout << "IntervallTimer created" << std::endl;    
    th = std::make_unique<std::thread>(&IntervallTimer::run, this);
  };


  void IntervallTimer::run() {
    
    std::cout << "tread created" << std::endl;

    while(1) {
      auto now = Timer::getCurrentTime();
      
      IntervallTimer::mtx.lock();
      for(auto m = t.begin(); m < t.end(); m++){
        if((m->lastCalled + m->period) <= now) {
          m->cbs();
          m->lastCalled = now;
          m->numCalls--;
          if(m->numCalls <= 0){
            //m = t.erase(m);
          }
        }
      }
      IntervallTimer::mtx.unlock();
      if(t.size() == 0){
        return;
      }

      std::this_thread::sleep_for(50ms);
    }
  }

  IntervallTimer& IntervallTimer::instance() {
    static IntervallTimer i; 
    return i;
  }


  void IntervallTimer::make_intervall(std::time_t  intervall, std::function<void()> cb, int calls) {
    
    Timers nt;
    nt.period = intervall;
    nt.cbs = cb;
    nt.numCalls = calls;
    nt.lastCalled = Timer::getCurrentTime();
    
    //  add timer to queue (we need an async thread here to allow timers beeing created in timer events)
    std::thread p([nt]() {
       IntervallTimer::instance().mtx.lock();
       IntervallTimer::instance().t.push_back(nt);
       IntervallTimer::instance().mtx.unlock();
    });
    p.detach();
  }

}