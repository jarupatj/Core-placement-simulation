#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() {
   srand(time(NULL));
}

RandomGenerator::~RandomGenerator() {
}

double RandomGenerator::uniform_0_1() {
   return rand() / (double) RAND_MAX;
}

int RandomGenerator::uniform_n(int n) {
   assert(n > 0);

   return uniform_0_1() * (n-1);
   //return uniform_0_1() * (n-1) + 1; //from [1,...,n]
}
