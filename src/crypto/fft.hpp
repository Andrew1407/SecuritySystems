#pragma once
#include <complex>

namespace fft {
  void fftFn(std::complex<double>*, std::complex<double>*, int);
  void ifftFn(std::complex<double>*, std::complex<double>*, int);
}
