#include <vector>
#include <iostream>
#include <sstream>
#include <complex>
#include <functional>
#include "./crypto.hpp"
#include "./fft.hpp"

using namespace std;

typedef complex<double> cmpVal;

uint64_t fftOperation(
  const uint64_t&,
  const uint64_t&,
  function<cmpVal(cmpVal, cmpVal)>
);
void splitDigits(uint64_t, vector<int8_t>*);

namespace crypto {
  uint64_t fftAdd(uint64_t a, uint64_t b) {
    return fftOperation(a, b, [] (cmpVal a, cmpVal b) {
      return a + b;
    });
  }

  uint64_t fftMul(uint64_t a, uint64_t b) {
    return fftOperation(a, b, [] (cmpVal a, cmpVal b) {
      return a * b;
    });
  }

  uint64_t fftPow(uint64_t a, uint64_t b) {
    bool aGreater = a > b;
    uint64_t result = 1;
    for (uint64_t i = 0; i < (aGreater ? b : a); ++i)
      result = crypto::fftMul(result, aGreater ? a : b);
    return result;
  }

  void execFft(string *value) {
    uint argsStart = value->find('(');
    string fnType = value->substr(0, argsStart);
    uint argsSep = value->find(',');
    string valStr1 = value->substr(argsStart + 1, argsSep - argsStart - 1);
    if (value->at(argsSep + 1) == ' ') ++argsSep;
    string valStr2 = value->substr(argsSep + 1, value->find(')') - argsSep - 1);
    uint64_t a, b;
    istringstream issA(valStr1), issB(valStr2);
    issA >> a;
    issB >> b;

    if (fnType == "fftAdd") {
      uint64_t result = crypto::fftAdd(a, b);
      cout << "Result: " << result << endl;
    } else if (fnType == "fftMul") {
      uint64_t result = crypto::fftMul(a, b);
      cout << "Result: " << result << endl;
    } else if (fnType == "fftPow") {
      uint64_t result = crypto::fftPow(a, b);
      cout << "Result: " << result << endl;
    } else {
      cout << "No specified: " << fnType << endl;
    }
  }
}

uint64_t fftOperation(
  const uint64_t &a,
  const uint64_t &b,
  function<cmpVal(cmpVal, cmpVal)> composer
) {
  vector<int8_t> aVec, bVec;
  splitDigits(a, &aVec);
  splitDigits(b, &bVec);
  uint maxLen = max(aVec.size(), bVec.size());
  uint resLen = pow(2, maxLen);
  cmpVal aIn[resLen], bIn[resLen], resultIn[resLen];
  cmpVal aOut[resLen], bOut[resLen], resultOut[resLen];
  for (int i = 0; i < resLen; ++i) {
    aIn[i] = i < aVec.size() ? aVec[i] : 0;
    bIn[i] = i < bVec.size() ? bVec[i] : 0;
  }
  fft::fftFn(aIn, aOut, maxLen);
  fft::fftFn(bIn, bOut, maxLen);
  for (int i = 0; i < resLen; ++i) resultIn[i] = composer(aOut[i], bOut[i]);
  fft::ifftFn(resultIn, resultOut, maxLen);
  uint64_t result = 0;
  for (int i = 0; i < resLen; ++i)
    result += pow(10, i) * (int)(resultOut[i].real() + 0.5);
  return result;
}

void splitDigits(uint64_t value, vector<int8_t> *container) {
  string valueStr = to_string(value);
  for (int i = valueStr.length() - 1; i >= 0; --i) {
    string d;
    d += valueStr[i];
    container->push_back(stoi(d));
  }
}
