#pragma once 
#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>

namespace yellowfortyfourcom {
  class Timer {
    public:
      Timer() = delete;
      static std::time_t getCurrentTime();

    private:

  };
}