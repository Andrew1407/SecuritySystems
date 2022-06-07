#pragma once
#include <string>

namespace crypto {
  // FFT
  uint64_t fftAdd(uint64_t, uint64_t);
  uint64_t fftMul(uint64_t, uint64_t);
  uint64_t fftPow(uint64_t, uint64_t);
  void execFft(std::string*);

  // Prime generator
  uint64_t getPrimeSSL();
  
  // RSA
  void generateUserKeys(std::string*);
  void removeUserKeys(std::string*);
  void encrypt(std::string, std::string, std::string);
  std::string decrypt(std::string, std::string);
  void decrypt(std::string, std::string, std::string);
  void execEncryption(std::string*, std::string*);
  void execDecryption(std::string*, std::string*);
}
