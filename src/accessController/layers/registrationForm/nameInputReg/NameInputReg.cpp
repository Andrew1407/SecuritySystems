#include <iostream>
#include <algorithm>
#include "./NameInputReg.hpp"
#include "../passwordInputReg/PasswordInputReg.hpp"
#include "../../../input/input.hpp"
#include "../../../../cmd/cmd.hpp"
#include "../../../../dataparser/dataparser.hpp"

using namespace std;

NameInputReg::NameInputReg(Privilege type): userType(type) {
  this->getUsernames();
}

bool NameInputReg::run() {
  bool valid = input::getUsername(this->name);
  if (!valid) return valid;
  if (!this->nameExists()) return true;
  cout << "Entered name has already been taken" << endl;
  return false;
}

void NameInputReg::next(Layer *&next) {
  next = new PasswordInputReg(this->userType, this->name);
}

void NameInputReg::getUsernames(){
  cmd::execWithRules([this] () {
    try {
      this->usernames = dataparser::getUsernames();
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::READ | Modifiers::EXECUTE);
}

bool NameInputReg::nameExists() {
  auto *users = &this->usernames; 
  return count(users->begin(), users->end(), this->name);
}
