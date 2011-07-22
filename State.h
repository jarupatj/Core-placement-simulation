#pragma once

#include <vector>

#include "Core.h"

using std::vector;

class State {
   public:
      //variable
      int cost;

      //function
      State();
      State(const State& sourceState); //copy constructor
      State& operator=(const State& sourceState); 
      ~State();

      void init();
      
      void generateNewState();

      void printAddr();
      
   private:
      //variable
      int **bandwidth;
      int **latency;
      int meshSize;

      int numCore;//total number of core
      vector<Core> core; 

      //function
      void calculateCost();
};
