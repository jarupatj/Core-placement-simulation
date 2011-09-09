/*
 * Utils.cpp
 *
 *  Created on: Aug 10, 2011
 *      Author: nana
 */

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "Defs.hpp"

int getHops(Coordinate a, Coordinate b) {
   return (int)(fabs(a.x - b.x) + fabs(a.y - b.y));
}

double uniform_0_1() {
   return (double) rand() / RAND_MAX;
}

int uniform_n(int n) {
   assert(n > 0);
   return (int) (uniform_0_1() * (n));
   //return uniform_0_1() * (n-1) + 1; //from [1,...,n]
}
