// Consteval Riemann zeta © 2026 David Flater BSL-1.0

#pragma once

#include <cmath>
#include <cstdint>

typedef _Float128         Rzfloat;
typedef Rzfloat _Complex  Rzcomplex;
#define RzRe(x)           (__real__ x)
#define RzIm(x)           (__imag__ x)

// Neither std::pow nor cpowq is constexpr yet.
consteval Rzcomplex Rzpow (uint8_t x, Rzcomplex y) {
  const Rzfloat lx = std::log((Rzfloat)x),
               phi = RzIm(y) * lx,
                 r = std::exp(RzRe(y) * lx);
  return {r*std::cos(phi), r*std::sin(phi)};
}

/*
  Implementation of Borwein algorithm 3 based on zeta_polynomial_series from
  the Math library in Boost 1.92.0.  Slightly different forms appear in
  boost/math/special_functions/zeta.hpp (© John Maddock 2007, 2014) and
  boost/math/constants/calculate_constants.hpp (© John Maddock 2010, 2012,
  and Paul A. Bristow 2011, 2012).  See README.

  The only reason n has to be a template parameter is to permit the use of
  static_assert to limit its range.
*/
template <uint8_t n> consteval Rzcomplex Rzzeta (Rzcomplex s) {
  // ej_term * (nx2 - j) overflows uint64_t at n = 63.
  static_assert(n < 63, "n is too large");
  static_assert(n > 0,  "n is too small");

  constexpr uint8_t nx2 = n << 1;
  constexpr Rzfloat pow2n = 1UL << n;
  Rzcomplex sum = {0, 0};
  int8_t ej_sign = 1;
  for (uint8_t j = 0; j < n; ++j) {
    sum += ej_sign * -pow2n / Rzpow(j+1, s);
    ej_sign = -ej_sign;
  }
  uint64_t ej_sum = 1, ej_term = 1;
  for (uint8_t j = n; j < nx2; ++j) {
    sum += ej_sign * (ej_sum - pow2n) / Rzpow(j+1, s);
    ej_sign = -ej_sign;
    ej_term = ej_term * (nx2 - j) / (j - n + 1);
    ej_sum += ej_term;
  }
  return -sum / (pow2n * (1 - Rzpow(2, 1-s)));
}
