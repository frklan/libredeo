#pragma once

#include <iostream>

namespace yellowfortyfourcom {
  
  const std::string APP_VERSION = "v0.0.1";

  class App {
    public:
      App() = delete;
      App(int arvc, char** argv);

      int run();
  };
}