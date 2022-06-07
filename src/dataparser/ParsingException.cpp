#include "./dataparser.hpp"

using namespace std;

namespace dataparser {
  ParsingException::ParsingException(const string &message) noexcept:
    message(message), extra("") { }

  ParsingException::ParsingException(const string &message, const string &extra) noexcept:
    message(message), extra(extra) { }

  const char* ParsingException::what() const noexcept {
    return this->message.c_str();
  }

  string ParsingException::whatExtended() const noexcept {
    string total = "Error";
    if (!this->extra.empty())
      total += " [" + this->extra + "]";
    total += ": " + this->message;
    return total;
  }
}
