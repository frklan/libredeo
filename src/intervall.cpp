#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <exception>
#include <future>

#include "timerutils.h"
#include "intervall.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  std::timed_mutex IntervallTimer::mtx;
  std::vector<Timer> timers;

  IntervallTimer::IntervallTimer()
  { 
    // runner is our thread that launches each timer event
    // will be destroyd in the destructor
    runner = std::thread(&IntervallTimer::run, this);
  };

  IntervallTimer::~IntervallTimer() {
    if(runner.joinable() && stop ==  false) {
      stop = true;
      runner.join();
    }
  }

  IntervallTimer& IntervallTimer::instance() noexcept {
    static IntervallTimer intervallTimer;
    return intervallTimer;
  }


  void IntervallTimer::run() {
    while(!stop) {
      std::unique_lock lock(IntervallTimer::mtx);
      for(auto t = timers.begin(); t < timers.end(); t++) {
        auto now = TimerUtils::getCurrentTime();
        auto exp = t->lastCalled + t->period.count();
        if(exp <= now) {
          auto f = std::thread([t](){ t->cbs(); });
          f.detach();
          t->lastCalled = now;
          t->numCalls--;
        }
        if(t->numCalls == 0) {
          t = timers.erase(t);
        }
      }
      std::this_thread::sleep_for(1ms);
    }
  }

  intervall_id IntervallTimer::make_intervall(std::chrono::seconds intervall, std::function<void()> cb, int16_t calls) noexcept {
    Timer nt;
    nt.period = intervall;
    nt.cbs = cb;
    nt.numCalls = calls;
    nt.lastCalled = TimerUtils::getCurrentTime();
    nt.timerId = IntervallTimer::getNextTimerId();

    auto t = std::thread([nt](){ 
      std::unique_lock lock(IntervallTimer::mtx);
      IntervallTimer::instance().timers.push_back(nt); 
    });
    t.join();
    
    return nt.timerId;
  }

  intervall_id IntervallTimer::make_timer(std::chrono::seconds delay, std::function<void()> cb) noexcept {
    return make_intervall(delay, cb, 1);
  }

  void IntervallTimer::cancel_timer(intervall_id id) noexcept {
    std::unique_lock lock(IntervallTimer::mtx);
    
    // find timer..
    auto t = std::find_if(IntervallTimer::instance().timers.begin(), IntervallTimer::instance().timers.end(), [id](Timer const& other) {
      return other.timerId == id;
    });

    // ..remove it from our vector
    if(t != IntervallTimer::instance().timers.end()) {
      IntervallTimer::instance().timers.erase(t);
    }
  }

  void IntervallTimer::cancel_all_timers() noexcept {
    std::unique_lock lock(IntervallTimer::mtx);
    IntervallTimer::instance().timers.clear();
  }


  void IntervallTimer::wait() noexcept {  
    while(IntervallTimer::instance().timers.size() != 0) {
      std::this_thread::sleep_for(1ms);
    }
  }
       
  intervall_id IntervallTimer::getNextTimerId() noexcept {
    static intervall_id id = 0;
    return id++;
  }
}