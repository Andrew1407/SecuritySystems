#include <iostream>
#include <sstream>
#include "../tools.hpp"
#include "../cmd/cmd.hpp"
#include "./crypto.hpp"

using namespace std;

namespace crypto {
  uint64_t getPrimeSSL() {
    string cmdStr = "openssl prime -generate -bits " + to_string(PRIME_SIZE_BYTES);
    string value = cmd::execWithOutput(cmdStr.c_str());
    istringstream iss(value);
    uint64_t result;
    iss >> result;
    return result;
  }
}
