#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "./inputMode.hpp"
#include "../../../cmd/cmd.hpp"
#include "../../../tools.hpp"
#include "../../../crypto/crypto.hpp"
#include "../../../functionCheck/functionCheck.hpp"
#include "../../../logger/logger.hpp"

using namespace std;

constexpr uint MONITORING_PERIOD_SECONDS = MONITORING_PERIOD_DAYS * 3600 * 24;

InputMode::InputMode(dataparser::User user): user(user), isQuestioning(false), input("") {
  this->getJournalData();
  cout << "Input mode:" << endl;
}

bool InputMode::run() {
  cout << this->getStatus();
  getline(cin, this->input);
  if (this->isQuestioning) {
    this->checkAnswer();
    return false;
  }
  if (cmd::isFftCmd(&this->input)) {
    crypto::execFft(&this->input);
    return false;
  }
  if (cmd::isPrimeGenCmd(&this->input)) {
    cout << crypto::getPrimeSSL() << endl;
    return false;
  }
  if (cmd::isEncryptCmd(&this->input)) {
    cmd::execWithRules([this] () {
      crypto::execEncryption(&this->input, &this->user.name);
    }, Modifiers::READ | Modifiers::EXECUTE);
    cmd::resetAccess(this->user.privilege);
    return false;
  }
  if (cmd::isDecryptCmd(&this->input)) {
    cmd::execWithRules([this] () {
      crypto::execDecryption(&this->input, &this->user.name);
    }, Modifiers::READ | Modifiers::EXECUTE);
    cmd::resetAccess(this->user.privilege);
    return false;
  }

  string cmdOut = cmd::execWithOutput(this->input.c_str());
  cout << cmdOut;
  bool denied = cmd::isDenied(&cmdOut);
  if (denied) this->notifyOfExeption();
  return false;
}

void InputMode::next(Layer *&next) {
  next = nullptr;
}

void InputMode::sendQuestion() {
  srand(time(NULL));
  this->isQuestioning = true;
  int variant = rand() % 100;
  this->fnChech = variant < 15;
  if (this->fnChech) {
    float x, y;
    functionCheck::generateValue(&this->expression, x, y);
    this->answer = to_string(x);
    cout << "\nVerification! Sercet function f(x) = " << to_string(y) << " (round up 2 minimum):" << endl; 
  } else {
    int i = rand() % this->user.questions.size();
    dataparser::Question q = this->user.questions[i];
    this->answer = q.answer;
    cout << "\nVerification! Give an answer to the question number " << i + 1 << ':' << endl << q.text << '?' << endl;
  }
  this->input.clear();
}

void InputMode::checkAnswer() {
  if (!this->isQuestioning) return;
  this->isQuestioning = false;
  try {
    bool failed;
    if (this->fnChech) {
      if (this->input.empty()) {
        failed = true;
      } else {
        float expected = stof(this->answer);
        float result = stof(this->input);
        result = ((int)(result * 100) / 100.0);
        failed = expected != result;
      }
    } else {
      failed = this->answer != this->input;
    }

    if (!failed) cout << "Verification passed" << endl;
    else this->onFailure();
  } catch (...) {
    this->onFailure();
  }
}

void InputMode::getJournalData() {
  cmd::execWithRules([this] () {
    try {
      this->expression = dataparser::getSecretFunction();
      this->monitorUsers();
    } catch (const dataparser::ParsingException &e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::READ | Modifiers::WRITE | Modifiers::EXECUTE);
  cmd::resetAccess(this->user.privilege);
}

string InputMode::getStatus() {
  string result = "[";
  result += static_cast<char>(user.privilege);
  result += " " + user.name + "]# ";
  return result;
}

void InputMode::onFailure() {
  --this->user.accessTries;
  cmd::execWithRules([this] () {
    try {
      if (this->user.accessTries) {
        dataparser::resetUserParams(&this->user);
        cout << "Verification failed; you should relogin (" << this->user.accessTries << " tries left)" << endl;
      } else {
        dataparser::removeUser(&this->user.name);
        crypto::removeUserKeys(&this->user.name);
        cout << "Verification failed; you have no retries; sign up again" << endl;
      }
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
    }
  }, Modifiers::READ | Modifiers::WRITE | Modifiers::EXECUTE);
  cmd::chmod(Modifiers::READ, DISK_PATH);
  exit(EXIT_FAILURE);
}

void InputMode::notifyOfExeption() {
  cmd::execWithRules([this] () {
    try {
      uint exceptionLevel = 1;
      string curTime = logger::getCurrentTime();
      dataparser::logExeption(&this->user.name, &curTime, exceptionLevel);
    } catch (const dataparser::ParsingException &e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::READ | Modifiers::WRITE | Modifiers::EXECUTE);
  cmd::resetAccess(this->user.privilege);
}

void InputMode::monitorUsers() {
  string loggerLastChecked = dataparser::getMonitoringLastCheck();
  bool expired = logger::checkTimeExpired(&loggerLastChecked, MONITORING_PERIOD_SECONDS);
  if (!expired) return;
  auto usersBehavior = dataparser::checkBehavior();
  string curTime = logger::getCurrentTime();
  dataparser::setMonitoringLastCheck(&curTime);
  dataparser::logSuspiciousBehavior(&usersBehavior);
}
