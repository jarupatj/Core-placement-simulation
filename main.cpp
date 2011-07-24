#include <iostream>

#include "SimulatedAnnealing.hpp"

using namespace std;

int main(int argc, char* argv[]) {

   SimulatedAnnealing sa;

   sa.init();

   sa.run();

   sa.printResult();

   return 0;
}
