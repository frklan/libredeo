#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <ctime>
#include <functional>
#include <mutex>

#include "timer.h"



namespace yellowfortyfourcom {
  
  const std::string APP_VERSION = "v0.0.1";

  class App {
    public:
      App() = delete;
      App(int arvc, char** argv);

      int run();

    private:
      std::unique_ptr<Timer> t;
  };
}