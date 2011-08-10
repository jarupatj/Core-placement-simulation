#include <iostream>

#include "SimulatedAnnealing.hpp"

using namespace std;

int main(int argc, char* argv[]) {

   if (argc != 10) {
      cout << "usage ./sa <alpha> <beta> <gamma> <theta> <starting_temp> <cooling rate>"
            " <iteration/temp> <input file>" << endl;
      return 0;
   }

   SimulatedAnnealing sa;

   int err = sa.init(argv);
   if(err) {
      cout << "Error Exit" << endl;
      return 0;
   }

   cout << "Initial State" << endl;
   sa.printSummary();

   cout << endl;

   sa.initTable();

   sa.run();

   cout << endl;
   sa.printSummary();

   return 0;
}
