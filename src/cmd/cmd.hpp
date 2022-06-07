#pragma once
#include <string>
#include <functional>
#include "../tools.hpp"

namespace cmd {
  void chmod(uint, std::string);
  void execWithRules(std::function<void()>, uint);
  void resetAccess(Privilege);
  std::string execWithOutput(char const*);
  bool isDenied(std::string*);
  bool isFftCmd(std::string*);
  bool isPrimeGenCmd(std::string*);
  bool isEncryptCmd(std::string*);
  bool isDecryptCmd(std::string*);
}
