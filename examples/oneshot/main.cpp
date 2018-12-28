#include <iostream>

// #include <libredeo/intervall.h>
#include "../../src/intervall.h"

using namespace std::chrono_literals;
using namespace yellowfortyfourcom;

int main(int argc, char* argv[]) {
  auto intervall = 5s;

  // IntervallTimer::make_timer is a short hand for a timer that fires only once.
  auto id = IntervallTimer::make_timer(intervall, [](){ std::clog << "Timer event\n"; });

  // Cancel any timer by calling IntervallTimer::cancel_timer() with the corresponding timer id
  // IntervallTimer::cancel_timer(id);

  // Unless we call IntervallTimer::wait() the application will exit and the timers
  // will be terminated by the runtime, possibly with a seg fault.
  IntervallTimer::wait();

  return 0;
}