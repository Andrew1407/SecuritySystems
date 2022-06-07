#pragma once
#include <string>

const std::string DISK_PATH = "disk-model";
const std::string ADMIN_FOLDER = DISK_PATH + "/.admin";
const std::string JOURNAL_PATH = ADMIN_FOLDER + "/journal.txt";
const std::string MONITORING_LOG_PATH = ADMIN_FOLDER + "/monitoring.log";
const std::string PRIVATE_KEY_PATH = ADMIN_FOLDER + "/pr.pem";
const std::string PUBLIC_KEY_PATH = ADMIN_FOLDER + "/pub.pem";

constexpr uint PASSWD_LEN_MIN = 4;
constexpr uint PASSWD_LEN_MAX = 26;
constexpr uint REG_FAILURE_LIMIT = 10;
constexpr uint MAX_USERS = 5;
constexpr uint QUESTIONS_COUNT = 4;
constexpr uint ANSWER_TIME_MIN = 1;
constexpr uint PASSWD_REFRESH_PERIOD_DAYS = 3;
constexpr uint KEYS_REFRESH_PERIOD_DAYS = 4;
constexpr uint MONITORING_PERIOD_DAYS = 1;
constexpr uint USER_EXCEPTIONS_LIMIT = 12;
constexpr uint PRIME_SIZE_BYTES = 48;
constexpr uint KEY_SIZE_BYTES = 8192;

const std::string ALLOWED_FOLDERS[] = { "A", "E" };

enum Privilege {
  ADMIN = 'a',
  USER = 'u',
  PRIVILEGED_USER = 'p'
};

enum Modifiers {
  NONE = 0b000,
  EXECUTE = 0b001,
  WRITE = 0b010,
  READ = 0b100
};
