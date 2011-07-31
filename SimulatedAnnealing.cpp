#include <iostream>
#include <cmath>

#include "SimulatedAnnealing.hpp"

SimulatedAnnealing::SimulatedAnnealing() {
}

SimulatedAnnealing::~SimulatedAnnealing() {
}

int SimulatedAnnealing::init(char* filename) {
   //init const
   TEMP_STEP = 100;
   MAX_STATE_CHANGE_PER_TEMP = 200;
   SUCCESS_PER_TEMP = 50;
   TEMP_CHANGE_FACTOR = 0.9;

   temp = 0.5;
   //init currentState
   int err = currentState.init(filename, random);
   if( err != 0 ) {
      return err;
   }
   return 0;
}

void SimulatedAnnealing::run() {
   int changeCost;
   int numSuccess = 0;

   std::cout << "start sa" << std::endl;

   for(int tStep = 0; tStep < TEMP_STEP; tStep++) {
      numSuccess = 0;
      for(int numChange = 0; numChange < MAX_STATE_CHANGE_PER_TEMP; numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState(random);
         /*
         std::cout << "--- newState ---" << std::endl;
         newState.printState();
         */
         changeCost = newState.cost - currentState.cost;

         //check legality
         if( newState.isLegal() && changeCost < 0) {
            currentState = newState; //deep copy
            numSuccess++;
         } else {
            if( acceptChange(changeCost) ) {
               currentState = newState; //deep copy
               numSuccess++;
            }
         }
         
         if( numSuccess > SUCCESS_PER_TEMP ) {
            std::cout << "*** success change temp ***" << std::endl;
            break;
         }
      }
      std::cout << tStep << std::endl;
      currentState.printState();
      temp = temp * TEMP_CHANGE_FACTOR;
   }

   std::cout << "finished sa" << std::endl;
}

int SimulatedAnnealing::acceptChange(int cost) {
   double r = random.uniform_0_1();
   double prob = exp( -(double)cost / temp );

   return r < prob;
}

void SimulatedAnnealing::printResult() {
   currentState.printState();
}
