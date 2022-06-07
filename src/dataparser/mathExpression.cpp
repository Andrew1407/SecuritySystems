#include <sstream>
#include "../crypto/crypto.hpp"
#include "./dataparser.hpp"

using namespace std;

namespace dataparser {
  string getSecretFunction() {
    string read = crypto::decrypt(JOURNAL_PATH, PRIVATE_KEY_PATH);
    if (read.empty())
      throw ParsingException("no expression found", "bad value");
    istringstream entries(read);
    string line;
    getline(entries, line);
    return line;
  };
}
