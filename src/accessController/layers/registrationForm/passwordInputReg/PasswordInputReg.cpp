#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "./PasswordInputReg.hpp"
#include "../questionAsk/QuestionAsk.hpp"
#include "../../../input/input.hpp"
#include "../../../../dataparser/dataparser.hpp"

using namespace std;

PasswordInputReg::PasswordInputReg(Privilege type, string name): userType(type), name(name) {}

bool PasswordInputReg::run() {
  bool isAdmin = this->userType == Privilege::ADMIN;
  int neccessaryLen = isAdmin ? PASSWD_LEN_MIN * 2 : PASSWD_LEN_MIN;
  bool valid = input::getPassword(this->password, neccessaryLen, "Password: ");
  return valid;
}

void PasswordInputReg::next(Layer *&next) {
  next = new QuestionAsk(this->userType, this->name, this->password);
}
