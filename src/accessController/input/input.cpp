#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <regex>
#include <map>
#include "../../tools.hpp"
#include "./input.hpp"

using namespace std;

string hiddenInput();
bool checkInput(string*, string&);

namespace input {
  bool getUsername(string &inputStr) {
    cout << "Username: ";
    getline(cin >> ws, inputStr);
    if (!inputStr.length()) return false;
    if (!regex_match(inputStr, regex("\\S+"))) {
      cout << "Username shouldn't contain spaces" << endl;
      return false;
    }
    return true;
  }

  bool getPassword(string &inputStr, uint minLen, char const *msg) {
    cout << msg;
    inputStr = hiddenInput();
    uint inputLen = inputStr.length();
    if (inputLen < minLen || inputLen > PASSWD_LEN_MAX) {
      cout << "Password size should be [" << minLen << ", " <<
        PASSWD_LEN_MAX << "] symbols. Try again." << endl;
      return false;
    }
    string warning;
    bool correct = checkInput(&inputStr, warning);
    if (!correct) {
      cout << warning << endl;
      return false;
    }
    return true;
  }
}

string hiddenInput() {
  termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  string inputStr;
  getline(cin, inputStr);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  cout << endl;
  return inputStr;
}

bool checkInput(string *value, string &warning) {
  map<char const*, char const*> rules = {
    { "Input shouldn't contain spaces", "\\S+" },
    { "No lower case letters contained", "^.*[a-z]+.*$" },
    { "No upper case letters contained", "^.*[A-Z]+.*$" },
    { "No digits contained", "^.*\\d+.*$" },
    { "No special characters contained", "^.*[!@#:;,\\.\\?\\*\\+-/~=\\[\\]\\{\\}\\(\\)\"']+.*$" }
  };
  
  for (auto &rule : rules) {
    bool matches = regex_match(value->c_str(), regex(rule.second));
    if (matches) continue;
    warning = rule.first;
    return false;
  }
  return true;
}
