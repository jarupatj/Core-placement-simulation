#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "RandomGenerator.hpp"

RandomGenerator::RandomGenerator() {
   //srand(time(NULL));
   srand(10);
}

RandomGenerator::~RandomGenerator() {
}

double RandomGenerator::uniform_0_1() {
   return (double) rand() / RAND_MAX;
}

int RandomGenerator::uniform_n(int n) {
   assert(n > 0);

   return (int) (uniform_0_1() * (n));
   //return uniform_0_1() * (n-1) + 1; //from [1,...,n]
}
