#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "SimulatedAnnealing.hpp"

using namespace std;

SimulatedAnnealing::SimulatedAnnealing() {}

SimulatedAnnealing::~SimulatedAnnealing() {}

int SimulatedAnnealing::init(char* argv[]) {
   temp = atof(argv[5]);
   END_TEMP = atof(argv[6]);
   TEMP_CHANGE_FACTOR = atof(argv[7]);
   MAX_STATE_CHANGE_PER_TEMP = atoi(argv[8]);

   //init currentState
   int err = currentState.init(atof(argv[1]), atof(argv[2]), \
                               atof(argv[3]), atof(argv[4]), \
                               argv[9], random);
   if( err != 0 ) {
      return err;
   }
   bestState = currentState;
   return 0;
}

void SimulatedAnnealing::run() {
   int changeCost, worstAcc, trial = 0;
   bool setCurrent = false;

   while( temp > END_TEMP ) {
      worstAcc = 0;
      trial++;
      for(int numChange = 0; numChange < MAX_STATE_CHANGE_PER_TEMP; numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState(random);
         changeCost = newState.getCost()- currentState.getCost();

         //check legality
         if( newState.isLegal() ) {
            if( changeCost < 0 ) {
               setCurrent = true;
            } else if( acceptChange(changeCost) ) {
               setCurrent = true;
               worstAcc++;
            }
         } 

         if( setCurrent ) {
            setCurrent = false;
            currentState = newState; //deep copy
            if( newState.isLegal() && currentState.getCost() <= bestState.getCost()) {
               bestState = currentState;
            }
         } 
      }

      printState(trial, worstAcc);
      temp = temp * TEMP_CHANGE_FACTOR;
   }
}

bool SimulatedAnnealing::acceptChange(int cost) {
   return isAccept(cost);
}

bool SimulatedAnnealing::isAccept(double value) {
   double r = random.uniform_0_1();
   double prob = exp( -value/ temp );
   return r < prob;
}

void SimulatedAnnealing::initTable() {
   cout << setw(6) << "trial"
   << setw(10) << "temp"
   << setw(12) << "cost"
   << setw(12) << "compaction"
   << setw(12) << "dilation"
   << setw(12) << "slack"
   << setw(12) << "proximity"
   << setw(12) << "util"
   << setw(12) << "worst acc" << endl;
}

void SimulatedAnnealing::printState(const int& trial, const int& worstAcc) {
   cout << setw(6) << trial;
   cout << setw(10) << setiosflags(ios::fixed) << setprecision(3) << temp;
   bestState.printState();
   cout << setw(12) << worstAcc << endl;
}

void SimulatedAnnealing::printSummary() {
   cout << "Temperature: " << temp << endl;
   bestState.printSummary();
}
