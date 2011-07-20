#include <iostream>

#include "SimulatedAnnealing.h"

SimulatedAnnealing::SimulatedAnnealing() {
}

SimulatedAnnealing::~SimulatedAnnealing() {
}

void SimulatedAnnealing::init() {
   //init const
   TEMP_STEP = 100;
   MAX_STATE_CHANGE_PER_TEMP = 30;
   SUCCESS_PER_TEMP = 10;
   TEMP_CHANGE_FACTOR = 0.9;

   //init linkLatency
   //init linkBandwidth

   temp = 50;
   //init currentState
}

void SimulatedAnnealing::run() {
   State *newState;

   for(int tStep = 0; tStep < TEMP_STEP; tStep++) {
      for(int numChange = 0; numChange < MAX_STATE_CHANGE_PER_TEMP; numChange++) {
         State *newState = new State(currentState); //deep copy
         newState->generateNewState();
         changeCost = newState->cost - currentState->cost;

         if( acceptChange(changeCost, temp) ) {
            delete currentState;
            currentState = NULL;
            currentState = newState; //deep copy
            numSuccess++;
         }
         
         if( numSuccess > SUCCESS_PER_TEMP ) {
            break;
         }
      }
      temp = temp * TEMP_CHANGE_FACTOR;
   }
}

void SimulatedAnnealing::changeState() {
}

void SimulatedAnnealing::acceptChange(int cost, double temp) {
   double r = random->uniform_0_1();
   double prob = exp( -(double)cost / temp );

   return cost < 0 || r < prob;
}
