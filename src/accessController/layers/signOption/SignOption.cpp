#include <iostream>
#include "./SignOption.hpp"
#include "../registrationForm/userType/UserType.hpp"
#include "../loginForm/nameInputLog/NameInputLog.hpp"
#include "../../../dataparser/dataparser.hpp"
#include "../../../cmd/cmd.hpp"

using namespace std;

SignOption::SignOption(): registration(false) {
  this->checkSignLimit();
}

bool SignOption::run() {
  cout << "Sign up (r) / sign in (l): ";
  string input;
  cin >> input;
  if (input == "r") {
    if (this->signedUsers < MAX_USERS) {
      this->registration = true;
      return true;
    } else {
      cout << "Maximum count of users is exceeded so you cannot sign up" << endl;
      return false;
    }
  } else if (input == "l") {
    if (!this->signedUsers) {
      cout << "There are no registered users" << endl;
      return false;
    }
    this->registration = false;
    return true;
  } else {
    cout << "Please, write a valid option" << endl;
    return false;
  }
}

void SignOption::next(Layer *&next) {
  if (this->registration) next = new UserType();
  else next = new NameInputLog(); 
}

void SignOption::checkSignLimit() {
  cmd::execWithRules([this] () {
    try {
      this->signedUsers = dataparser::getUsernames().size();
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::READ | Modifiers::EXECUTE);
}
