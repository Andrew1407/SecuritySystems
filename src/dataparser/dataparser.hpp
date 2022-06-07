#pragma once
#include <string>
#include <vector>
#include <map>
#include "../tools.hpp"

namespace dataparser {
  // User data
  typedef struct {
    std::string text;
    std::string answer;
  } Question;

  typedef std::vector<Question> questionsVec;

  typedef struct {
    std::string name;
    std::string password;
    Privilege privilege;
    std::string rerfeshPasswordDate;
    std::string rerfeshKeysDate;
    questionsVec questions;
    uint accessTries = REG_FAILURE_LIMIT;
    
    std::string toString();
  } User;
  
  void saveUser(User*);
  void resetUserParams(User*);
  void removeUser(std::string*);
  std::vector<std::string> getUsernames();
  User findUser(std::string*);

  // Math expression
  std::string getSecretFunction();

  // Monitoring
  std::string getMonitoringLastCheck();
  void setMonitoringLastCheck(std::string*);
  void logExeption(std::string*, std::string*, uint);
  std::map<std::string, int> checkBehavior();
  void logSuspiciousBehavior(std::map<std::string, int>*);

  // Parsing exception
  class ParsingException : public std::exception {
    public:
      ParsingException(const std::string&) noexcept;
      ParsingException(const std::string&, const std::string&) noexcept;
      virtual ~ParsingException() = default;
      virtual const char *what() const noexcept override;
      std::string whatExtended() const noexcept;
    private:
      std::string message;
      std::string extra;
      std::string total;
  };
}
