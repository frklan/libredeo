#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include "timerutils.h"
#include "intervall.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  IntervallTimer::IntervallTimer()
  { 
    std::cout << "IntervallTimer created" << std::endl;    
    timerThread = std::make_unique<std::thread>(&IntervallTimer::run, this);
  };

  void IntervallTimer::run() {
    std::cout << "timer tread created" << std::endl;

    while(1) {
      auto now = TimerUtils::getCurrentTime();
      
      IntervallTimer::mtx.lock();
      for(auto m = timers.begin(); m < timers.end(); m++){
        if((m->lastCalled + m->period.count()) <= now) {
          m->cbs();
          m->lastCalled = now;
          
          if(m->numCalls == -1) {
            continue;
          } else {
          m->numCalls--;
            if(m->numCalls == 0){
              m = timers.erase(m);
            }
          }
        }
      }
      IntervallTimer::mtx.unlock();
      
      if(timers.size() == 0){
        return;
      }

      std::this_thread::sleep_for(50ms);
    }
  }

  IntervallTimer& IntervallTimer::instance() {
    static IntervallTimer intervallTimer;
    return intervallTimer;
  }


  void IntervallTimer::make_timer(std::chrono::seconds delay, std::function<void()> cb) {
    make_intervall(delay, cb, 1);
  }

  /**
   * Add a new timer to the scheduling queue
   * The first timer event will be current time + intervall
   * 
   * */
  void IntervallTimer::make_intervall(std::chrono::seconds intervall, std::function<void()> cb, int16_t calls) {
    Timer nt;
    nt.period = intervall;
    nt.cbs = cb;
    nt.numCalls = calls;
    nt.lastCalled = TimerUtils::getCurrentTime();
    
    //  add timer to queue (we need an async thread here to allow timer callbacks to create timer new timers)
    std::thread p([nt]() {
       IntervallTimer::instance().mtx.lock();
       IntervallTimer::instance().timers.push_back(nt);
       IntervallTimer::instance().mtx.unlock();
    });
    p.detach();
  }
}