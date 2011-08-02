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
   int changeCost, changeIllegal;
   int numSuccess = 0;

   std::cout << "start sa" << std::endl;

   for(int tStep = 0; tStep < TEMP_STEP; tStep++) {
      numSuccess = 0;
      for(int numChange = 0; numChange < MAX_STATE_CHANGE_PER_TEMP; numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState(random);
         std::cout << "--- new state ---\n";
         newState.printState();
         changeCost = newState.cost - currentState.cost;
         changeIllegal = newState.illegalCount - currentState.illegalCount;

         //check legality
         if( currentState.illegalCount != 0 && newState.illegalCount == 0 ) {
            currentState = newState; //deep copy
            numSuccess++;
            std::cout << "accept\n";
         } else if( (newState.illegalCount == 0) && (changeCost < 0) ) {
            currentState = newState; //deep copy
            numSuccess++;
            std::cout << "accept\n";
         } else {
            if( acceptChange(changeCost, changeIllegal ) ) {
               currentState = newState; //deep copy
               numSuccess++;
               std::cout << "accept\n";
            }
         }
         
         if( numSuccess > SUCCESS_PER_TEMP ) {
            std::cout << "*** success change temp ***" << std::endl;
            break;
         }
      }
      std::cout << "tstep = " << tStep << std::endl;
      currentState.printState();
      temp = temp * TEMP_CHANGE_FACTOR;
   }

   std::cout << "finished sa" << std::endl;
}

bool SimulatedAnnealing::acceptChange(int cost, int illegal) {
   if( cost < 0 ) {
      return isAccept((double)illegal);
   } else if( illegal != 0 ) {
      return isAccept((double)cost);
   } else {
      return (isAccept((double)cost) && isAccept((double)illegal));
   }
   //return isAccept((double)cost);
   //return isAccept((double)illegal);
}

bool SimulatedAnnealing::isAccept(double value) {
   double r = random.uniform_0_1();
   double prob = exp( -value/ temp );
   return r < prob;
}

void SimulatedAnnealing::printResult() {
   currentState.printState();
}
