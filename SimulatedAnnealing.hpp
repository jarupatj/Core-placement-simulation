#pragma once

#include "RandomGenerator.hpp"
#include "State.hpp"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      int init(char* filename);
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
      bool acceptChange(int cost, int illegal);
      bool isAccept(double value);
};
