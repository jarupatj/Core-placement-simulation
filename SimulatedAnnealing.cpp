#include <iostream>
#include <cmath>

#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing() {
}

SimulatedAnnealing::~SimulatedAnnealing() {
}

void SimulatedAnnealing::init() {
   //init const
   TEMP_STEP = 1;
   MAX_STATE_CHANGE_PER_TEMP = 400;
   SUCCESS_PER_TEMP = 40;
   TEMP_CHANGE_FACTOR = 0.9;

   temp = 0.5;
   //init currentState
   currentState.init();
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
         //check legality
         changeCost = newState.cost - currentState.cost;

         if( newState.isLegal() && changeCost < 0) {
            currentState = newState; //deep copy
            currentState.printState();
            numSuccess++;
         } else {
            if( acceptChange(changeCost) ) {
               currentState = newState; //deep copy
               currentState.printState();
               numSuccess++;
            }
         }
         
         if( numSuccess > SUCCESS_PER_TEMP ) {
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
