#include <array>
#include <charconv>
#include <cmath>
#include <complex>
#include <quadmath.h>

// Situation as of gcc version 17.0.0 20260518 (experimental)

// std::pow is not constexpr
constexpr auto a =
  std::pow(std::complex<double>(1.0, 2.0), std::complex<double>(3.0, 4.0));

// cpowq is not constexpr
constexpr auto b =
  cpowq((_Float128 _Complex){1.0q, 2.0q}, (_Float128 _Complex){3.0q, 4.0q});

// logf128 is not constexpr
constexpr auto c = logf128(1.0f128);

// logq is not constexpr
constexpr auto d = logq(1.0q);

// std::riemann_zeta is not constexpr
constexpr auto e = std::riemann_zeta(1.0);

// std::to_chars is not constexpr
consteval int to_chars_test () {
  std::array<char, 10> buf;
  std::to_chars(buf.data(), buf.data() + 10, 1.0);
  return 1;
}
constexpr auto f = to_chars_test();

// cabsq is not constexpr
constexpr auto g = cabsq((_Float128 _Complex){1.0q, 2.0q});

// std::abs does not exist for _Float128 _Complex
constexpr auto h = std::abs((_Float128 _Complex){1.0q, 2.0q});

// std::abs is constexpr for a double but not for std::complex<double>
constexpr auto i = std::abs(std::complex<double>(1.0, 2.0));
