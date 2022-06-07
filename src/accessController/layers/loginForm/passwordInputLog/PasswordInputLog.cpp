#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "./PasswordInputLog.hpp"
#include "../../diskReader/DiskReader.hpp"
#include "../../../input/input.hpp"
#include "../../../../cmd/cmd.hpp"
#include "../../diskReader/DiskReader.hpp"
#include "../../../../tools.hpp"
#include "../../../../logger/logger.hpp"
#include "../passwordReset/PasswordReset.hpp"
#include "../keysRefreshSubmission/KeysRefreshSubmission.hpp"

using namespace std;

constexpr uint PASSWD_EXPIRATION_PERIOD_SECONDS = PASSWD_REFRESH_PERIOD_DAYS * 3600 * 24;
constexpr uint KEYS_EXPIRATION_PERIOD_SECONDS = KEYS_REFRESH_PERIOD_DAYS * 3600 * 24;

PasswordInputLog::PasswordInputLog(dataparser::User user): user(user), tries(REG_FAILURE_LIMIT) {
  this->passwdExpired = logger::checkTimeExpired(&user.rerfeshPasswordDate, PASSWD_EXPIRATION_PERIOD_SECONDS);
  this->keysExpired = logger::checkTimeExpired(&user.rerfeshKeysDate, KEYS_EXPIRATION_PERIOD_SECONDS);
  bool isAdmin = this->user.privilege == Privilege::ADMIN;
  this->necessaryLength = isAdmin ? PASSWD_LEN_MIN * 2 : PASSWD_LEN_MIN;
  if (this->passwdExpired)
    cout << "Your password terms is expired (max. period: " << PASSWD_REFRESH_PERIOD_DAYS
      << " days); last reset: " << user.rerfeshPasswordDate << endl;
}

bool PasswordInputLog::run() {
  bool valid = input::getPassword(this->passwordInput, this->necessaryLength, "Password: ");
  if (valid && this->passwordInput == this->user.password) {
    if (!this->passwdExpired)
      cout << "User \"" << this->user.name << "\" has successfully logged in." << endl;
    return true;
  }
  if(--this->tries) {
    cout << "Invalid password (" << this->tries << " tries left)" << endl;
    return false;
  };
  cout << "All tries have been used, exiting" << endl;
  exit(1);
}

void PasswordInputLog::next(Layer *&next) {
  if (this->passwdExpired)
    next = new PasswordReset(this->user, this->necessaryLength, this->keysExpired);
  else if (this->keysExpired)
    next = new KeysRefreshSubmission(this->user);
  else
    next = new DiskReader(this->user);
}
