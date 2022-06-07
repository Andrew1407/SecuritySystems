#include "DiskReader.hpp"
#include <cstdint>
#include <iostream>
#include <filesystem>
#include "../../../cmd/cmd.hpp"
#include "../inputMode/inputMode.hpp"

using namespace std;

uint16_t handleAccess(Privilege);
bool isAllowed(string*);
void setEntryModifiers(bool, string*, uint16_t);

DiskReader::DiskReader(dataparser::User user): user(user) {
  this->modifiers = handleAccess(user.privilege);
}

bool DiskReader::run() {
  cout << "Available entries: " << endl;
  for (const auto &entry : filesystem::directory_iterator(DISK_PATH)) {
    string path = entry.path();
    this->handlePathAccess(&path);
  }
  return true;
}

void DiskReader::next(Layer *&next) {
  next = new InputMode(this->user);
}

void DiskReader::handlePathAccess(string *path) {
  switch (this->user.privilege) {
    case Privilege::ADMIN: {
      cmd::chmod(this->modifiers, *path);
      cout << *path << endl;
      return;
    }
    case Privilege::USER: {
      bool allowed = isAllowed(path);
      setEntryModifiers(allowed, path, this->modifiers);
      return;
    }
    case Privilege::PRIVILEGED_USER: {
      bool allowed = *path != ADMIN_FOLDER;
      setEntryModifiers(allowed, path, this->modifiers);
      return;
    }
  }
}

uint16_t handleAccess(Privilege userType) {
  uint16_t modifiers = Modifiers::READ | Modifiers::EXECUTE;
  if (userType != Privilege::USER) modifiers |= Modifiers::WRITE;
  cmd::chmod(modifiers, DISK_PATH);
  return modifiers;
}

bool isAllowed(string *path) {
  for (const string f : ALLOWED_FOLDERS) {
    string fullPathF = DISK_PATH + "/" + f;
    if (fullPathF == *path) return true;
  }
  return false;
}

void setEntryModifiers(bool allowed, string *path, uint16_t modifiers) {
  uint16_t mod;
  if (*path == ADMIN_FOLDER) mod = Modifiers::NONE;
  else mod = allowed ? modifiers : Modifiers::EXECUTE;
  cmd::chmod(mod, *path);
  if (allowed) cout << *path << endl;
}

