# Rzzeta:  calculate and plot Riemann zeta at compile time

    g++ -std=c++26 -Wall -Wextra -pedantic -c Rzplot.cc
    Rzplot.cc:82:15: error: static assertion failed: 
     26 ******************                           11111111
     25 ******************         0         1111111111111111
     24 *****************                            11111111
     23 *****************                             1111111
     22 *****************                             1111111
     21 ****************           0                111111111
     20 ****************                          11111111111
     19 ***************                               1111111
     18 ***************                               1111111
     17 **************                                1111111
     16 *************                           1111111111111
     15 ************                                 11111111
     14 ***********                0                  1111111
     13 **********                                    1111111
     12 *********                                   111111111
     11 *******                                    1111111111
     10 *****                                         1111111
      9 ***                                           1111111
      8 *                                            11111111
      7                                          111111111111
      6                                             111111111
      5                                               1111111
      4                                               1111111
      3                                              11111111
      2                                   1      111111111111
      1                                               1111111
      0   0   0   0   0   0   0     *                 1111111
     -1                                               1111111
     -2                                   1      111111111111
     -3                                              11111111
     -4                                               1111111
     -5                                               1111111
     -6                                             111111111
     -7                                          111111111111
     -8 *                                            11111111
     -9 ***                                           1111111
    -10 *****                                         1111111
    -11 *******                                    1111111111
    -12 *********                                   111111111
    -13 **********                                    1111111
    -14 ***********                0                  1111111
    -15 ************                                 11111111
    -16 *************                           1111111111111
    -17 **************                                1111111
    -18 ***************                               1111111
    -19 ***************                               1111111
    -20 ****************                          11111111111
    -21 ****************           0                111111111
    -22 *****************                             1111111
    -23 *****************                             1111111
    -24 *****************                            11111111
    -25 ******************         0         1111111111111111
    -26 ******************                           11111111

       82 | static_assert(false, plotstring);
	  |               ^~~~~
    make: *** [Makefile:7: plot] Error 1

## Overview

Rzzeta is a C++26 demo/stunt that generates an ASCII plot of the magnitude of
the [Riemann zeta function
ζ](https://en.wikipedia.org/wiki/Riemann_zeta_function) at compile time.  It
works in suitably recent g++.

The requirement for C++26 comes from the use of `static_assert` with a
non-string message in the plotting function, not from complex math.  The
std::complex class is not even used.

The hard part was discovering and working around library functions that could
or should have been declared constexpr but were not, beginning with
std::riemann_zeta (though that is furthermore limited to real numbers).
Refer to not_constexpr_fns.cc for the litany of failures or run `make
testfns` to see the carnage.

## Zeta algorithm

The algorithm is called algorithm 3 in P. Borwein,
An efficient algorithm for the Riemann zeta function, in Michel
Théra (Ed.), Constructive, Experimental, and Nonlinear Analysis, Canadian
Mathematical Society Conference Proceedings Vol. 27, pp. 29–34, 2000.
ISBN-13: 9780821821671.
[Preprint](https://www.cecm.sfu.ca/~pborwein/PAPERS/P155.pdf),
[Internet Archive](https://archive.org/details/constructiveexpe0000unse_c6t5)

Borwein's algorithm 3 is the simplest of a group of simple algorithms that
perform well in proximity to the origin but become increasingly useless at
greater "heights" (increasing distance from the real axis).  Its nice
features include not requiring a gamma function to calculate results for the
negative half-plane.

Borwein provides separate absolute error bounds for points in the positive
and negative half-planes.  These of course do not account for floating point
error in the implementation.  The imaginary axis is covered by neither
equation, but this appears to be an omission of the error bounds and not a
region where the algorithm breaks down.

## Zeta implementation

The implementation is based on zeta_polynomial_series from the Math library
in Boost 1.92.0.  Slightly different forms appear in
boost/math/special_functions/zeta.hpp (© John Maddock 2007, 2014) and
boost/math/constants/calculate_constants.hpp (© John Maddock 2010, 2012, and
Paul A. Bristow 2011, 2012).  The most significant changes were generalizing
it to complex numbers and using integer types for the coefficients.  Those
coefficients are fixed for a given n, so they could even be tabulated instead
of calculated, but the calculation is simple and compact.

A third implementation of zeta appears in riemann_zeta_impl of
boost/decimal/detail/cmath/riemann_zeta.hpp, but it uses Borwein's algorithm
2 instead of 3.

## Accuracy

Accuracy improves with larger values of the n parameter until floating point
error takes over.  Using the theoretical error bounds provided by Borwein,
the worst points in the positive and negative half-planes that appear in the
plot are 0.5±26i and -13±26i respectively.  The following table shows the
theoretical error bounds and the difference with [Alexey
Kuznetsov's](https://github.com/DaveFlater/AK_Riemann_zeta_gamma_C) zeta
implementation in absolute and relative terms for those two points using n =
38.  Kuznetsov's implementation does not use Borwein's algorithm, but I
haven't verified that there isn't some other common factor that would make it
agree too much with Rzzeta.

 s | abs err bound | rel err bound | abs diff | rel diff
--- | --- | --- | --- | ---
0.5+26i | 1.335821e-15 | 9.366079e-16 | 2.050700e-22 | 1.437843e-22
-13+26i | 9.530637e+05 | 2.559596e-03 | 2.136252e-05 | 5.737227e-14

## License

To avoid a licensing fiasco, I have applied the Boost license to Rzzeta as a
whole.  Kindly give attribution where attribution is due.
