#include <iostream>
#include <cstdarg>
#include <fstream>
#include <memory>
#include <cstdio>
#include <algorithm>
#include "./cmd.hpp"

using namespace std;

namespace cmd {
  void chmod(uint modifiers, std::string path) {
    string m = to_string(modifiers);
    string cmdStr = "chmod " + m + m + m + ' ' + path;
    system (cmdStr.c_str());
  }

  void execWithRules(function<void()> fn, uint modifiers) {
    chmod(modifiers, DISK_PATH);
    chmod(modifiers, ADMIN_FOLDER);
    fn();
    modifiers = Modifiers::NONE;
    chmod(modifiers, ADMIN_FOLDER);
    modifiers = Modifiers::READ;
    chmod(modifiers, DISK_PATH);
  }

  void resetAccess(Privilege p) {
    if (p != Privilege::USER)
      cmd::chmod(Modifiers::READ | Modifiers::WRITE | Modifiers::EXECUTE, DISK_PATH);
    else
      cmd::chmod(Modifiers::READ | Modifiers::EXECUTE, DISK_PATH);

    if (p == Privilege::ADMIN)
      cmd::chmod(Modifiers::READ | Modifiers::WRITE | Modifiers::EXECUTE, ADMIN_FOLDER);
    else
      cmd::chmod(Modifiers::NONE, ADMIN_FOLDER);
  }

  string execWithOutput(char const *cmdStr) {
    string cmdFull = cmdStr;
    cmdFull.append(" 2>&1");
    shared_ptr<FILE> pipe(popen(cmdFull.c_str(), "r"), pclose);
    if (!pipe) throw runtime_error("popen() failed!");;
    char buffer[128];
    string result = "";
    while (!feof(pipe.get())) {
      if (fgets(buffer, 128, pipe.get()))
        result += buffer;
    }
    return result;
  }

  bool isDenied(string *cmdStr) {
    string ending = ": Permission denied\n";
    if (cmdStr->length() <= ending.length()) return false;
    return !cmdStr->compare(cmdStr->length() - ending.length(), ending.length(), ending);
  }

  bool isFftCmd(std::string *value) {
    return value->find("fft") == 0;
  }

  bool isPrimeGenCmd(std::string *value) {
    return *value == "generatePrime()";
  }

  bool isEncryptCmd(string *value) {
    return value->find("encrypt") == 0;
  }

  bool isDecryptCmd(string *value) {
    return value->find("decrypt") == 0;
  }
}
