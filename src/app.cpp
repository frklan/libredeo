#include <iostream>
#include <functional>
#include <vector>
#include <mutex>
#include "app.h"
#include "intervall.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

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