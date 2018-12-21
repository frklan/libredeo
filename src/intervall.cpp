#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <exception>

#include "timerutils.h"
#include "intervall.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  std::timed_mutex IntervallTimer::mtx;

  IntervallTimer::IntervallTimer()
  { 
    std::cout << "IntervallTimer created" << std::endl;    
    timerThread = std::make_unique<std::thread>(&IntervallTimer::run, this);
  };

  void IntervallTimer::run() {
    std::cout << "timer thread created" << std::endl;

    while(1) {
      auto now = TimerUtils::getCurrentTime();
      
      std::unique_lock lock(IntervallTimer::mtx, 1ms);
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
      lock.unlock();
      
      if(timers.size() == 0){
        return;
      }

      std::this_thread::sleep_for(1ms);
    }
  }

  IntervallTimer& IntervallTimer::instance() noexcept {
    static IntervallTimer intervallTimer;
    return intervallTimer;
  }


  intervall_id IntervallTimer::make_timer(std::chrono::seconds delay, std::function<void()> cb) noexcept {
    return make_intervall(delay, cb, 1);
  }

  /**
   * Add a new timer to the scheduling queue
   * The first timer event will be current time + intervall
   *
   * returns an Id of the timer, the id can be used in cancel_timer()
   * */
  intervall_id IntervallTimer::make_intervall(std::chrono::seconds intervall, std::function<void()> cb, int16_t calls) noexcept {
    Timer nt;
    nt.period = intervall;
    nt.cbs = cb;
    nt.numCalls = calls;
    nt.lastCalled = TimerUtils::getCurrentTime();
    nt.timerId = IntervallTimer::getNextTimerId();
    
    //  add timer to queue (we need an async thread here to allow timer callbacks to create timer new timers)
    std::thread p([nt]() {
      std::unique_lock lock(IntervallTimer::mtx);
      IntervallTimer::instance().timers.push_back(nt);
    });
    p.detach();

    return nt.timerId;
  }

  void IntervallTimer::cancel_timer(intervall_id id) {
    
    
    std::thread p([id]() {
      bool idFound = false;
      //IntervallTimer::instance().mtx.lock();
      std::unique_lock lock(IntervallTimer::mtx);
      for(auto it = IntervallTimer::instance().timers.begin(); it < IntervallTimer::instance().timers.end(); it++) {
        if(it->timerId == id) {
          it = IntervallTimer::instance().timers.erase(it);
          idFound = true;
          break;
        }
      }
      //IntervallTimer::instance().mtx.unlock();
      if(!idFound) {
        throw std::runtime_error("Timer not found!");
      }
    });
    p.detach();
  }

  intervall_id IntervallTimer::getNextTimerId() noexcept {
    static intervall_id id = 1;

    return ++id;
  }
}