#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <future>

namespace yellowfortyfourcom {
  using intervall_id = uint16_t;
  
  struct Timer {
    std::chrono::seconds  period;
    std::time_t lastCalled;
    int16_t numCalls;
    std::function<void()> cbs;
    intervall_id timerId;
  };
  
  class IntervallTimer {
    public:
      static intervall_id make_timer(std::chrono::seconds delay, std::function<void()> callback) noexcept;
      static intervall_id make_intervall(std::chrono::seconds intervall, std::function<void()> callback, int16_t calls = 1) noexcept;
      static void cancel_timer(intervall_id id) noexcept;
      static void cancel_all_timers() noexcept;
      static void wait() noexcept;
      
    private:
       IntervallTimer();
      ~IntervallTimer();
      
      static IntervallTimer& instance() noexcept;
      static intervall_id getNextTimerId() noexcept;
      void run();
      
      static std::timed_mutex mtx;
      std::vector<Timer> timers;
      std::thread runner;
      bool stop = false;
  };
}