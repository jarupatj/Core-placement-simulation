#include <iostream>
#include <iomanip>
#include <cmath>

#include "SimulatedAnnealing.hpp"

using namespace std;

SimulatedAnnealing::SimulatedAnnealing() {}

SimulatedAnnealing::~SimulatedAnnealing() {}

int SimulatedAnnealing::init(char* filename) {
   //init const
   MAX_STATE_CHANGE_PER_TEMP = 200;
   TEMP_CHANGE_FACTOR = 0.9;
   END_TEMP = 0.1;

   temp = 1000.0;
   //init currentState
   int err = currentState.init(filename, random);
   if( err != 0 ) {
      return err;
   }
   bestState = currentState;
   return 0;
}

void SimulatedAnnealing::run() {
   int changeCost;
   bool setCurrent = false;

   cout << "start sa" << endl;

   cout << setw(10) << "temp"
   << setw(12) << "cost"
   << setw(12) << "compaction"
   << setw(12) << "dilation"
   << setw(12) << "slack"
   << setw(12) << "proximity"
   << setw(12) << "util" << endl;

   while( temp > END_TEMP ) {
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
      printState();
      temp = temp * TEMP_CHANGE_FACTOR;
   }

   cout << "finished sa" << endl;
}

bool SimulatedAnnealing::acceptChange(int cost) {
   return isAccept(cost);
}

bool SimulatedAnnealing::isAccept(double value) {
   double r = random.uniform_0_1();
   double prob = exp( -value/ temp );
   return r < prob;
}

void SimulatedAnnealing::printState() {
   cout << setw(10) << setiosflags(ios::fixed) << setprecision(3) << temp;
   bestState.printState();
}
