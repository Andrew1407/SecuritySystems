#include <ctime>
#include <iomanip>
#include <chrono>
#include "./logger.hpp"
#include "../tools.hpp"

using namespace std;

const char *TIME_FORMAT = "%a_%b_%d_%H:%M:%S_%Y";

time_t parseTime(string*);

namespace logger {
  string getCurrentTime() {
    stringstream transTime;
    time_t now = time(NULL);
    transTime << put_time(localtime(&now), TIME_FORMAT);
    return transTime.str();
  }

  bool checkTimeExpired(std::string *timeString, double const &period) {
    time_t timeToCheck = parseTime(timeString);
    time_t timeNow = time(NULL);
    double diff = difftime(timeNow, timeToCheck);
    bool expired = diff >= period;
    return expired;
  }
}

time_t parseTime(string* timeStr) {
  struct tm time;
  strptime(timeStr->c_str(), TIME_FORMAT, &time);
  return mktime(&time);
}
