#include <iostream>
#include "./NameInputLog.hpp"
#include "../passwordInputLog/PasswordInputLog.hpp"
#include "../../../input/input.hpp"
#include "../../../../dataparser/dataparser.hpp"
#include "../../../../cmd/cmd.hpp"

using namespace std;

NameInputLog::NameInputLog() {
  this->getUsernames();
}

bool NameInputLog::run() {
  bool valid = input::getUsername(this->name);
  if (!valid) return valid;
  if (this->nameExists()) return true;
  cout << "Entered name isn't registeged" << endl;
  return false;
}

void NameInputLog::next(Layer *&next) {
  dataparser::User user;
  cmd::execWithRules([this, &user] () {
    try {
      user = dataparser::findUser(&name);
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::READ | Modifiers::EXECUTE);

  next = new PasswordInputLog(user);
}

void NameInputLog::getUsernames(){
  cmd::execWithRules([this] () {
    try {
      this->usernames = dataparser::getUsernames();
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }

  }, Modifiers::READ | Modifiers::EXECUTE);
}

bool NameInputLog::nameExists() {
  auto *users = &this->usernames; 
  return count(users->begin(), users->end(), this->name);
}
