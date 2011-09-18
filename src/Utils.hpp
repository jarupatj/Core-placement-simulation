#ifndef UTILS_HPP
#define UTILS_HPP

#include "Defs.hpp"

int getHops(Coordinate a, Coordinate b);
/*
 * Generate uniform random number in [0,1] interval
 */
double uniform_0_1();
/*
 * Generate uniform random number in [0,n) internal
 * This does not include n.
 */
int uniform_n(int n);

#endif
