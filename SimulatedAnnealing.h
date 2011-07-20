#pragma once

#include "RandomGenerator.h"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      void init();

      void run();

   private:
      //constant
      int TEMP_STEP;
      int MAX_STATE_CHANGE_PER_TEMP;
      int SUCCESS_PER_TEMP;
      int TEMP_CHANGE_FACTOR;
      //all links assume to have same latency and bandwidth
      int LINK_LATENCY;
      int LINK_BANDWIDTH;

      //variable
      State currentState;
      RandomGenerator random;
      double temp;


      //function
      void changeState();
      int getCost();
      int acceptChange();
};
