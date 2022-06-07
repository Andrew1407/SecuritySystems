#include <iostream>
#include "./QuestionAsk.hpp"
#include "../../diskReader/DiskReader.hpp"
#include "../../../../cmd/cmd.hpp"
#include "../../../../crypto/crypto.hpp"
#include "../../../../logger/logger.hpp"

using namespace std;

QuestionAsk::QuestionAsk(Privilege privilege, string name, string password):
  userType(privilege), name(name), password(password) { }

bool QuestionAsk::run() {
  for (int i = 0; i < QUESTIONS_COUNT;) {
    string text;
    string answer;
    cout << "Enter a question number " << ++i << ": " << endl;
    cin >> text;
    cout << "Enter an answer to question " << i << ": " << endl;
    cin >> answer;
    this->querstions.push_back({ text, answer });
  }

  return true;
}

void QuestionAsk::next(Layer *&next) {
  this->makeUser();
  next = new DiskReader(this->user);
}

void QuestionAsk::makeUser() {
  string currentTime = logger::getCurrentTime();
  this->user = {
    .name = this->name,
    .password = this->password,
    .privilege = this->userType,
    .rerfeshPasswordDate = currentTime,
    .rerfeshKeysDate = currentTime,
    .questions = this->querstions,
  };

  cmd::execWithRules([this] () {
    try {
      dataparser::saveUser(&this->user);
      crypto::generateUserKeys(&this->user.name);
    } catch (const dataparser::ParsingException e) {
      cout << e.whatExtended() << endl;
      exit(EXIT_FAILURE);
    }
  }, Modifiers::WRITE | Modifiers::EXECUTE);
  
  cout << "User \"" << this->user.name << "\" has been registered" << endl;
}
