#include "UserType.hpp"
#include <iostream>
#include <string>
#include "../nameInputReg/NameInputReg.hpp"

using namespace std;

bool inputValid(char ch);

UserType::UserType() {
  string m = to_string(Modifiers::READ);
  string cmd = "chmod " + m + m + m;
  cmd += " " + DISK_PATH;
  system(cmd.c_str());
}

bool UserType::run() {
  string input;
  cout << "Choose the role (admin - a, common user - u, priviledged user - p): ";
  cin >> input;
  if (input.length() != 1) return false;
  char inputType = input[0];
  bool isValid = inputValid(inputType);
  if (!isValid) return false;
  this->type = static_cast<Privilege>(inputType);
  return true;
}

void UserType::next(Layer *&next) {
  next = new NameInputReg(this->type);
}

bool inputValid(char ch) {
  return ch == Privilege::USER ||
    ch == Privilege::PRIVILEGED_USER ||
    ch == Privilege::ADMIN;
}
