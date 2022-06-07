#include "../cmd/cmd.hpp"
#include "../tools.hpp"
#include "../dataparser/dataparser.hpp"
#include "./crypto.hpp"

using namespace std;

string getKeyPath(string*, bool);
void parseValues(string, string*, string&, string&);

namespace crypto {
  void generateUserKeys(string *name) {
    string pubKey = getKeyPath(name, true);
    string privateKey = getKeyPath(name, false);

    string keyCmd;
    keyCmd = "openssl genrsa -out ";
    keyCmd += privateKey + ' ' + to_string(KEY_SIZE_BYTES);
    cmd::execWithOutput(keyCmd.c_str());

    keyCmd = "openssl rsa -in ";
    keyCmd += privateKey + " -out " + pubKey + " -outform PEM -pubout";
    cmd::execWithOutput(keyCmd.c_str());
  }

  void removeUserKeys(string *name) {
    string pubKey = getKeyPath(name, true);
    string privateKey = getKeyPath(name, false);
    remove(pubKey.c_str());
    remove(privateKey.c_str());
  }

  void encrypt(string from, string to, string pubKey) {
    string command = "openssl rsautl -encrypt -inkey ";
    command += pubKey + " -pubin -in " + from + " -out " + to;
    system(command.c_str());
  }

  void decrypt(string from, string to, string privKey) {
    string command = "openssl rsautl -decrypt -inkey ";
    command += privKey + " -in " + from + " -out " + to;
    system(command.c_str());
  }

  string decrypt(string from, string privKey) {
    string command = "openssl rsautl -decrypt -inkey ";
    command += privKey + " -in " + from;
    return cmd::execWithOutput(command.c_str());
  }

  void execDecryption(string *value, string *username) {
    string keyPath = getKeyPath(username, false);
    string from, to;
    parseValues("decrypt", value, from, to);
    crypto::decrypt(from, to, keyPath);
  }

  void execEncryption(string *value, string *username) {
    string keyPath = getKeyPath(username, true);
    string from, to;
    parseValues("encrypt", value, from, to);
    crypto::encrypt(from, to, keyPath);
  }
}

string getKeyPath(string *value, bool isPublic) {
  string keyType = isPublic ? "-pub" : "-pr";
  return ADMIN_FOLDER + '/' + *value + keyType + ".pem";
}

void parseValues(string cmdType, string *value, string& from, string& to) {
  string values = value->substr(cmdType.length() + 1);
  uint separator = values.find(' ') + 1;
  from = values.substr(0, separator);
  to = values.substr(separator);
}
