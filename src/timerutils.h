#pragma once 
#include <ctime>

namespace yellowfortyfourcom {
  class TimerUtils {
    public:
      TimerUtils() = delete;
      static std::time_t getCurrentTime();
  };
}