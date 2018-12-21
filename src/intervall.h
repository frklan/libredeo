#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>
#include <vector>

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

   struct Timers {
    std::time_t  period;
    std::time_t lastCalled;
    int numCalls;
    std::function<void()> cbs;
  };

  
  class IntervallTimer {
    public:
      static void make_intervall(std::time_t  intervall, std::function<void()> callback, int calls = 1);
      static void wait() { IntervallTimer::instance().th->join(); };
      
    private:
      static IntervallTimer& instance();
      IntervallTimer();
      ~IntervallTimer() {};
      std::mutex mtx;

      std::unique_ptr<std::thread> th;
      std::vector<Timers> t;

      void run();
  };
    

}