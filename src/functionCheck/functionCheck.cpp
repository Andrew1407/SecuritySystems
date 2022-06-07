#include "./functionCheck.hpp"
#include "../../lib/mathexpr/mathexpr.h"
#include <stdlib.h>
#include <time.h>

using namespace std;

constexpr int A = 6;

float calcFunction(string*, float);

namespace functionCheck {
  void generateValue(string *expressionStr, float &x, float &y) {
    srand(time(NULL));
    x = (float)(rand() % 1000) / 100.0;
    x = (float)((int)(x * 100) / 100.0);
    y = calcFunction(expressionStr, x);
  }

  bool compareInput(string *expressionStr, float expected, float inputVal) {
    if (inputVal < -A) return false;
    return expected == calcFunction(expressionStr, inputVal);
  }
}

float calcFunction(string *expressionStr, float x) {
  double ad = A;
  double xd = x;
  RVar avar("a", &ad);
  RVar xvar("x", &xd);
  RVar* vararray[2];
  vararray[0] = &avar;
  vararray[1] = &xvar;
  ROperation op((char*)(expressionStr->c_str()), 2, vararray);
  return op.Val();
}
