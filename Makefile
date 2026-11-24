.EXTRA_PREREQS = Makefile

CXX = g++
CXXFLAGS = -std=c++26 -Wall -Wextra -pedantic

plot:
	$(CXX) $(CXXFLAGS) -c Rzplot.cc

testfns: not_constexpr_fns
not_constexpr_fns: not_constexpr_fns.cc
	g++ -std=gnu++26 -Wall -Wextra -c not_constexpr_fns.cc
