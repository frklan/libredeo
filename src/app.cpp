#include <iostream>
#include <chrono>
#include "app.h"
#include "intervall.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  App::App(int argc, char** argv) {

    IntervallTimer::make_intervall(1s, [](){std::cout << "timer 1" << std::endl; }, 150);
    std::this_thread::sleep_for(500ms);
    IntervallTimer::make_intervall(1s, [](){std::cout << "timer 2" << std::endl;}, 5);
    std::this_thread::sleep_for(750ms);
    IntervallTimer::make_intervall(12s, [](){
      std::cout << "new timer 3" << std::endl;
      IntervallTimer::make_intervall(2s, [](){ std::cout << "timer created within another timers callback" << std::endl; }, 3);
    }, 2);

    auto id = IntervallTimer::make_intervall(5s, [](){std::cout << "timer running for ever" << std::endl; }, -1);
  
    IntervallTimer::make_timer(20s, [id]() {
      std::cout << "canceling timer #" << id << std::endl;
      IntervallTimer::cancel_timer(id);
    });
    
  }

  int App::run() {
    // do stuff..

    IntervallTimer::wait();
    return 0;
  }
}