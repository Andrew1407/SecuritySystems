#include <fstream>
#include <algorithm>
#include <sstream>
#include "../crypto/crypto.hpp"
#include "./dataparser.hpp"

using namespace std;

dataparser::User parseUser(string*, vector<string>*);
void configureJournal(string*, string*);

namespace dataparser {
  string User::toString() {
    string stringified = this->name+ ' ';
    stringified += this->password + ' '; 
    stringified += (char)this->privilege;
    stringified += ' ' + to_string(this->accessTries);
    stringified += ' ' + this->rerfeshPasswordDate;
    stringified += ' ' + this->rerfeshKeysDate;
    stringified += '\n';
    for (int i = 0; i < QUESTIONS_COUNT; ++i) {
      Question *q = &this->questions[i];
      stringified += ' ' + q->text + "\n " + q->answer + '\n';
    }
    stringified += '\n';
    return stringified;
  }

  void saveUser(User *user) {
    string read = crypto::decrypt(JOURNAL_PATH, PRIVATE_KEY_PATH);
    string rawFile = JOURNAL_PATH + ".raw";
    ofstream file;
    if (!file)
      throw ParsingException("file not found, cannot save user", "writing to the journal");
    file.open(rawFile);
    string entries = read + user->toString();
    file << entries;
    file.close();
    crypto::encrypt(rawFile, JOURNAL_PATH, PUBLIC_KEY_PATH);
    remove(rawFile.c_str());
  }

  User findUser(std::string *name) {
    string read = crypto::decrypt(JOURNAL_PATH, PRIVATE_KEY_PATH);
    istringstream entries(read);
    string line;
    bool found = false;
    string params;
    vector<string> questions;
    bool firstLine = true;
    while (getline(entries, line))
      if (firstLine) {
        firstLine = false;
        continue;
      } else if (!found && line.rfind(*name, 0) == 0) {
        found = true;
        params = line;
      } else if (found) {
        if (line[0] != ' ') break;
        questions.push_back(line.substr(1));
      }

    if (params.empty() && questions.empty())
      throw ParsingException("user \"" + *name + "\" not found", "reading the journal");
    return parseUser(&params, &questions);
  }

  vector<string> getUsernames() {
    string read = crypto::decrypt(JOURNAL_PATH, PRIVATE_KEY_PATH);
    istringstream entries(read);
    string line;
    vector<string> names;
    bool firstLine = true;
    while (getline(entries, line)) {
      if (firstLine) {
        firstLine = false;
        continue;
      }
      if (line.empty()) continue;
      if (line[0] == ' ') continue;
      int i = 0;
      for (i; line[i] != ' '; ++i);
      names.push_back(line.substr(0, i));
    }

    return names;
  }

  void resetUserParams(User *user) {
    string userStr = user->toString();
    configureJournal(&user->name, &userStr);
  }

  void removeUser(std::string *name) {
    configureJournal(name, nullptr);
  }
}

dataparser::User parseUser(string *data, vector<string> *questionData) {
  vector<string> parsed;
  dataparser::questionsVec questions;
  int counter = 0;
  string val = "";

  for (int i = 0; i < data->length(); ++i) {
    char ch = data->at(i);
    if (ch != ' ') {
      val += ch;
      continue;
    }
    parsed.push_back(val);
    val.clear();
  }

  for (int i = 0; i < questionData->size(); i += 2) {
    questions.push_back({
      .text = questionData->at(i),
      .answer = questionData->at(i + 1),
    });
  }

  return {
    .name = parsed[0],
    .password = parsed[1],
    .privilege = static_cast<Privilege>(parsed[2][0]),
    .rerfeshPasswordDate = parsed[4],
    .rerfeshKeysDate = parsed[5],
    .questions = questions,
    .accessTries = static_cast<uint>(stoi(parsed[3])),
  };
}

void configureJournal(string *username, string *configurable) {
  string read = crypto::decrypt(JOURNAL_PATH, PRIVATE_KEY_PATH);
  string rawFile = JOURNAL_PATH + ".raw";
  istringstream readEntries(read);
  string entries = "";
  string line;
  bool firstLine = true;
  int counter = 0;
  while (getline(readEntries, line)) {
    if (firstLine) {
      firstLine = false;
      entries += line + '\n';
      continue;
    }
    if (line.rfind(*username, 0) == 0) {
      ++counter;
      if (configurable) entries += *configurable;
    } else if (counter > 0 && counter < 10) {
      ++counter;
    } else {
      entries += line + '\n';
    }
  }
  

  ofstream fileInput;
  if (!fileInput)
    throw dataparser::ParsingException("file not found, journal wasn't edited", "reading the journal");;
  fileInput.open(rawFile);
  fileInput << entries;
  fileInput.close();
  crypto::encrypt(rawFile, JOURNAL_PATH, PUBLIC_KEY_PATH);
  remove(rawFile.c_str());
}
