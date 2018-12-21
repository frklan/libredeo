#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include "timerutils.h"

namespace yellowfortyfourcom {
  std::time_t TimerUtils::getCurrentTime() {
    using sec = std::chrono::duration<int, std::ratio<1>>;
    auto now = std::chrono::time_point_cast<sec>(std::chrono::system_clock::now()).time_since_epoch();
   
    return now.count();
  }
}