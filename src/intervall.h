#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>
#include <vector>

namespace yellowfortyfourcom {
   struct Timer {
    std::chrono::seconds  period;
    std::time_t lastCalled;
    int16_t numCalls;
    std::function<void()> cbs;
  };

  
  class IntervallTimer {
    public:
      static void make_timer(std::chrono::seconds delay, std::function<void()> callback);
      static void make_intervall(std::chrono::seconds intervall, std::function<void()> callback, int16_t calls = 1);
      static void wait() { IntervallTimer::instance().timerThread->join(); };
      
    private:
       IntervallTimer();
      ~IntervallTimer() {};
      
      static IntervallTimer& instance();
      void run();

      std::mutex mtx;
      std::unique_ptr<std::thread> timerThread;
      std::vector<Timer> timers;      
  };
}