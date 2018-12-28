#include <iostream>

//#include <libredeo/intervall.h>
#include "../../src/intervall.h"

using namespace std::chrono_literals;
using namespace yellowfortyfourcom;

int main(int argc, char* argv[]) {
  auto intervall = 5s;
  auto repeats = 7;

  // Create a timer that fires once every [intervall], repeats [repeats] times.
  auto id = IntervallTimer::make_intervall(intervall, [](){ std::clog << "Timer event\n"; }, repeats);

  // Cancel any timer by calling IntervallTimer::cancel_timer() with the corresponding timer id
  // IntervallTimer::cancel_timer(id);

  // Unless we call IntervallTimer::wait() the application will exit and the timers
  // will be terminated by the runtime, possibly with a segfault.
  IntervallTimer::wait();

  return 0;
}