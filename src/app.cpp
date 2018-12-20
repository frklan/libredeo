#include <iostream>
#include <functional>
#include <vector>
#include <mutex>
#include "app.h"

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

  App::App(int argc, char** argv) {
    //auto at = Timer::getCurrentTime() + 30;
    //t = std::make_unique<Timer>(at, [](){std::cout << "time's up!" << std::endl;});

    IntervallTimer::make_intervall(5, [](){std::cout << "new timer 1" << std::endl;});
    std::this_thread::sleep_for(500ms);
    IntervallTimer::make_intervall(3, [](){std::cout << "new timer 2" << std::endl;}, 5);
    std::this_thread::sleep_for(750ms);
    IntervallTimer::make_intervall(7, [](){
      std::cout << "new timer 3" << std::endl;
      IntervallTimer::make_intervall(2, [](){ std::cout << "extra timer" << std::endl; }, 5);
    });
  
  }

  int App::run() {
    // do stuff..

    IntervallTimer::wait();
    return 0;
  }
}