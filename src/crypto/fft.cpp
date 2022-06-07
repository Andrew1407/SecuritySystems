#include <math.h>
#include "./fft.hpp"

using namespace std;

namespace fft {
  void fftFn(complex<double> *f, complex<double> *ftilde, int log2N) {
    int N = 1 << log2N;
    for (int i = 0; i < N; i++) {
      int ii = 0, x = i;
      for (int j = 0; j < log2N; j++) {
        ii <<= 1;
        ii |= ( x & 1 );
        x >>= 1;
      }
      ftilde[ii] = f[i];
    }

    for (int s = 1; s <= log2N; s++ ) {
      int m = 1 << s;
      int m2 = m / 2;
      complex<double> w = 1.0;
      complex<double> wm = polar(1.0, -M_PI / m2);
      for ( int j = 0; j < m2; j++ ) {
        for ( int k = j; k < N; k += m ) {
          complex<double> t = w * ftilde[k+m2];
          complex<double> u = ftilde[k];
          ftilde[k] = u + t;
          ftilde[k + m2] = u - t;
        }
        w *= wm;
      }
    }
  }

  void ifftFn( complex<double> *ftilde, complex<double> *f, int log2N) {
    int N = 1 << log2N;
    for (int m = 0; m < N; m++) ftilde[m] = conj(ftilde[m]);
    fft::fftFn(ftilde, f, log2N);
    double factor = 1.0 / N;
    for (int m = 0; m < N; m++) f[m] = conj(f[m]) * factor;
    for (int m = 0; m < N; m++) ftilde[m] = conj(ftilde[m]);
  }
}
