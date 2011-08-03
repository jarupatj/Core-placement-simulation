#include <iostream>
#include <cmath>

#include "SimulatedAnnealing.hpp"

SimulatedAnnealing::SimulatedAnnealing() {
}

SimulatedAnnealing::~SimulatedAnnealing() {
}

int SimulatedAnnealing::init(char* filename) {
   //init const
   TEMP_STEP = 30;
   MAX_STATE_CHANGE_PER_TEMP = 100;
   SUCCESS_PER_TEMP = 50;
   TEMP_CHANGE_FACTOR = 0.9;
   END_TEMP = 0.1;

   temp = 100.0;
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
   int numSuccess = 0;

   std::cout << "start sa" << std::endl;

   temp = 100.0;
   while( temp > END_TEMP ) {
      numSuccess = 0;
      for(int numChange = 0; numChange < MAX_STATE_CHANGE_PER_TEMP; numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState(random);
         std::cout << "--- new state ---\n";
         newState.printState();
         changeCost = newState.cost - currentState.cost;

         //check legality
         if( currentState.isLegal() ) {
            if( changeCost < 0 ) {
               setCurrent = true;
            } else if( acceptChange(changeCost) ) {
               setCurrent = true;
            }
         } 
         /*
         else {
            if( acceptChange(changeCost) ) {
               setCurrent = true;
            }
         }
         */

         if( setCurrent ) {
            setCurrent = false;
            currentState = newState; //deep copy
            numSuccess++;
            //std::cout << "accept\n";
            if( currentState.cost <= bestState.cost ) {
               bestState = currentState;
            }
         } 
      }
      std::cout << "temp = " << temp << std::endl;
      bestState.printState();
      temp = temp * TEMP_CHANGE_FACTOR;
   }

   std::cout << "finished sa" << std::endl;
}

bool SimulatedAnnealing::acceptChange(int cost) {
   return isAccept(cost);
}

bool SimulatedAnnealing::isAccept(double value) {
   double r = random.uniform_0_1();
   double prob = exp( -value/ temp );
   return r < prob;
}

void SimulatedAnnealing::printResult() {
   bestState.printState();
}
