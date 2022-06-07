#include <iostream>
#include "../../../../tools.hpp"
#include "../../../../cmd/cmd.hpp"
#include "../../../../crypto/crypto.hpp"
#include "../../../../dataparser/dataparser.hpp"
#include "../../../../logger/logger.hpp"
#include "../../diskReader/DiskReader.hpp"
#include "./KeysRefreshSubmission.hpp"

using namespace std;

KeysRefreshSubmission::KeysRefreshSubmission(dataparser::User user): user(user) { }

bool KeysRefreshSubmission::run() {
  string input;
  cout << "Your keys terms are expired. Would you like to refresh (y/n): ";
  cin >> input;
  if (input.length() != 1) return false;
  this->answer = input[0];
  if (this->answer != 'y' && this->answer != 'n') return false;
  return true;  
}

void KeysRefreshSubmission::next(Layer *&next) {
  this->handleKeysRefreshment();
  next = new DiskReader(this->user);
}

void KeysRefreshSubmission::handleKeysRefreshment() {
  if (this->answer == 'n') return;
  cmd::execWithRules([this] () {
    crypto::generateUserKeys(&this->user.name);
    this->user.rerfeshKeysDate = logger::getCurrentTime();
    dataparser::resetUserParams(&this->user);
  }, Modifiers::READ | Modifiers::WRITE | Modifiers::EXECUTE);
  cmd::resetAccess(this->user.privilege);
}
