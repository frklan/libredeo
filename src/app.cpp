#include <iostream>
#include <chrono>
#include "app.h"
#include "intervall.h"
#include <string>
#include "timerutils.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  App::App(int argc, char** argv) { }
  
  int App::run() {

    // Create a timer that fires once every second, repeats 10 times.
    auto t1 = IntervallTimer::make_intervall(1s, [](){ std::clog << "Timer event\n"; }, 10);
    std::this_thread::sleep_for(8s);
    
    // Cancel any timer by calling IntervallTimer::cancel_timer() with the corresponding timer id
    IntervallTimer::cancel_timer(t1);


    IntervallTimer::make_intervall(1s, [](){ std::clog << "A second timer firing\n"; }, 10);
    IntervallTimer::make_intervall(1s, [](){ std::clog << "A third timer firing\n"; }, 10);

    // IntervallTimer::make_timer is a short hand for a timer that fires only once.
    IntervallTimer::make_timer(5s, []() {

      // IntervallTimer::cancel_all_timers() will destry all active timers.
      IntervallTimer::cancel_all_timers();  
    });

    // Unless we call IntervallTimer::wait() the application will exit and the timers
    // will be terminated by the runtime, possibly with a seg fault.
    IntervallTimer::wait();
    return 0;
  }
}