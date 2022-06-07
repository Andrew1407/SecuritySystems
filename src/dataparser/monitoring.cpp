#include <fstream>
#include <algorithm>
#include <sstream>
#include "../crypto/crypto.hpp"
#include "./dataparser.hpp"

using namespace std;

namespace dataparser {
  string getMonitoringLastCheck() {
    ifstream file(MONITORING_LOG_PATH);
    string read = crypto::decrypt(MONITORING_LOG_PATH, PRIVATE_KEY_PATH);
    istringstream entries(read);
    string line;
    getline(entries, line);
    if (line.empty())
      throw ParsingException("no date found in log", "bad value");
    return line;
  }

  void setMonitoringLastCheck(std::string *date) {
    string rawFile = MONITORING_LOG_PATH + ".raw";
    ofstream file;
    if (!file)
      throw ParsingException("cannot edit log file", "writing to the monitoring log");
    file.open(rawFile);
    file << *date + '\n';
    file.close();
    crypto::encrypt(rawFile, MONITORING_LOG_PATH, PUBLIC_KEY_PATH);
    remove(rawFile.c_str());
  }

  void logExeption(string *name, string *date, uint level) {
    string read = crypto::decrypt(MONITORING_LOG_PATH, PRIVATE_KEY_PATH);
    string rawFile = MONITORING_LOG_PATH + ".raw";
    ofstream file;
    if (!file)
      throw ParsingException("file not found, cannot log", "writing to the monitoring log");
    file.open(rawFile);
    string logged = *name + ' ' + to_string(level) + ' ' + *date + '\n';
    file << read + logged;
    file.close();
    crypto::encrypt(rawFile, MONITORING_LOG_PATH, PUBLIC_KEY_PATH);
    remove(rawFile.c_str());
  }

  void logSuspiciousBehavior(map<string, int> *users) {
    string read = crypto::decrypt(MONITORING_LOG_PATH, PRIVATE_KEY_PATH);
    string rawFile = MONITORING_LOG_PATH + ".raw";
    ofstream file;
    if (!file)
      throw ParsingException("file not found, cannot log", "writing to the monitoring log");
    file.open(rawFile);
    string list = "";
    for (map<string, int>::iterator it = users->begin(); it != users->end(); ++it) {
      if (it->second >= USER_EXCEPTIONS_LIMIT)
        list += "[Suspicious behavior] " + it->first + " (" + to_string(it->second) + " times)\n";
    }

    file << read + list;
    file.close();
    crypto::encrypt(rawFile, MONITORING_LOG_PATH, PUBLIC_KEY_PATH);
    remove(rawFile.c_str());
  }

  map<string, int> checkBehavior() {
    string read = crypto::decrypt(MONITORING_LOG_PATH, PRIVATE_KEY_PATH);
    string rawFile = MONITORING_LOG_PATH + ".raw";
    map<string, int> failures;
    auto users = dataparser::getUsernames();
    istringstream entries(read);
    string line;
    bool first = true;
    while (getline(entries, line)) {
      if (first) {
        first = false;
        continue;
      }
      if (line.rfind("[Suspicious behavior] ", 0) == 0) continue;
      for (auto name : users) {
        if (line.rfind(name, 0) == 0) {
          if (failures.find(name) == failures.end()) failures[name] = 0;
          ++failures[name];
        };
      }
    }

    return failures;
  }
}
