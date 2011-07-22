#pragma once

#include "RandomGenerator.h"
#include "State.h"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      void init();
      void run();
      void printResult();

   private:
      //constant
      int TEMP_STEP;
      int MAX_STATE_CHANGE_PER_TEMP;
      int SUCCESS_PER_TEMP;
      int TEMP_CHANGE_FACTOR;

      //variable
      State currentState;
      RandomGenerator random;
      double temp;

      //function
      int getCost();
      int acceptChange(int cost);
};
