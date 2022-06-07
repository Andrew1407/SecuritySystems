#pragma once
#include <string>

namespace logger {

  // Time management
  std::string getCurrentTime();
  bool checkTimeExpired(std::string*, double const&);
}
