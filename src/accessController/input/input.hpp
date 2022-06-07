#pragma once
#include <string>

namespace input {
  bool getUsername(std::string&);
  bool getPassword(std::string&, uint, char const*);
}
