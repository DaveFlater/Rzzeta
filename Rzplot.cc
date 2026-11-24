// Plot for consteval Riemann zeta © 2026 David Flater BSL-1.0

#include "Rzzeta.hh"
#include <array>

/*
  cabsq is not constexpr yet.
  std::abs does not exist for _Float128 _Complex yet.
  std::abs is constexpr for a double but not yet for std::complex<double>.
  std::sqrt is constexpr only if the argument is finite.
*/
consteval Rzfloat Rzabs (Rzcomplex s) {
  const Rzfloat s2 = RzRe(s)*RzRe(s) + RzIm(s)*RzIm(s);
  if (!std::isfinite(s2))
    return INFINITY;
  return std::sqrt(s2);
}

/*
  Choose a plotting character given the magnitude and a hint whether it came
  from the critical line.  The only cases we care about are 0, 1, and large.
  The gimmick is setting the tolerance so that the first three nontrivial
  zeroes will show up even though the sampling grid is not aligned with them.
*/
consteval char makeasciipixel (Rzfloat mag, bool critical_line) {
  if (!std::isfinite(mag) || mag > 1000)
    return '*';
  if (critical_line && mag < 0.11)
    return '0';
  if (mag < 0.001)
    return '0';
  if (std::abs(mag - 1.0) < 0.001)
    return '1';
  return ' ';
}

/*
  Generate the plot.  Each line is:
     3 char:  imaginary axis label
     1 char:  space
    53 char:  plot
     1 char:  \n
  Total 58 char
  The buffer has 53 lines plus an initial \n.
*/
consteval auto Rzplot () {
  constexpr uint8_t linelen = 58, hdrlen = 1;
  constexpr uint16_t buflen = linelen * 53 + hdrlen;
  std::array<char, buflen> buf;
  buf[0] = '\n';

  for (int8_t i = -26; i <= 26; ++i) {
    const uint16_t linestart = linelen * (26 - i) + hdrlen;
    buf[linestart + linelen - 1] = '\n';

    // std::to_chars is not constexpr yet.
    if (i >= 0) {
      buf[linestart]   = ' ';
      buf[linestart+1] = (i > 9 ? i/10 + '0' : ' ');
      buf[linestart+2] = i%10 + '0';
    } else {
      const uint8_t ai = -i;
      if (ai > 9) {
        buf[linestart]   = '-';
        buf[linestart+1] = ai/10 + '0';
      } else {
        buf[linestart]   = ' ';
        buf[linestart+1] = '-';
      }
      buf[linestart+2] = ai%10 + '0';
    }
    buf[linestart+3] = ' ';

    for (int8_t rx2 = -26; rx2 <= 26; ++rx2)
      buf[linestart + 30 + rx2] = makeasciipixel(
        Rzabs(Rzzeta<38>({rx2/2.0f128, i})), rx2 == 1);
  }
  return buf;
}

constexpr auto plotstring = Rzplot();
static_assert(false, plotstring);
