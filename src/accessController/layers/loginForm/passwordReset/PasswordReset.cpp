#include <iostream>
#include "./PasswordReset.hpp"
#include "../../diskReader/DiskReader.hpp"
#include "../../../input/input.hpp"
#include "../../../../tools.hpp"
#include "../../../../dataparser/dataparser.hpp"
#include "../../../../cmd/cmd.hpp"
#include "../../../../logger/logger.hpp"
#include "../keysRefreshSubmission/KeysRefreshSubmission.hpp"

using namespace std;

PasswordReset::PasswordReset(dataparser::User user, uint passwdLen, bool keysExpired):
  user(user), necessaryLength(passwdLen), keysExpired(keysExpired), tries(REG_FAILURE_LIMIT) { }

bool PasswordReset::run() {
  bool valid = input::getPassword(this->input, this->necessaryLength, "New password: ");
  if (this->user.password == this->input) {
    cout << "Invalid password - try to make up a new one ("
      << --this->tries << " tries left)" << endl;
    return false;
  }
  if (valid) {
    cout << "User \"" << this->user.name << "\" has successfully logged in." << endl;
    return true;
  }
  if(--this->tries) {
    cout << "Invalid password (" << this->tries << " tries left)" << endl;
    return false;
  };
  cout << "All tries have been used, try again, exiting" << endl;
  exit(1);
}

void PasswordReset::next(Layer *&next) {
  this->refreshPassword();
  if (this->keysExpired)
    next = new KeysRefreshSubmission(this->user);
  else
    next = new DiskReader(this->user);
}

void PasswordReset::refreshPassword() {
  cmd::execWithRules([this] () {
    try {
      this->user.password = this->input;
      this->user.rerfeshPasswordDate = logger::getCurrentTime();
      dataparser::resetUserParams(&this->user);
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::WRITE | Modifiers::EXECUTE);
}
