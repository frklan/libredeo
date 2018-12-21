#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include "timer.h"

namespace yellowfortyfourcom {
  using namespace std::chrono_literals;

  std::time_t Timer::getCurrentTime() {
    using sec = std::chrono::duration<int, std::ratio<1>>;
    auto now = std::chrono::time_point_cast<sec>(std::chrono::system_clock::now()).time_since_epoch();
   
    return now.count();
  }
}