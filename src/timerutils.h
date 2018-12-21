#pragma once 
#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>

namespace yellowfortyfourcom {
  class TimerUtils {
    public:
      TimerUtils() = delete;
      static std::time_t getCurrentTime();
  };
}