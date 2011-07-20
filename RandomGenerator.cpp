#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

RandomGenerator::RandomGenerator() {
}

RandomGenerator::~RandomGenerator() {
}

double RandomGenerator::uniform_0_1() {
   srand(time(NULL));

   return rand() / (double) RAND_MAX;
}

int RandomGenerator::uniform_n(int n) {
   assert(n > 0);

   return uniform_0_1() * (n-1) + 1;
}
