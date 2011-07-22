#pragma once

#include <vector>

#include "Core.h"
#include "RandomGenerator.h"

#define LINK_LATENCY 10
#define LINK_BANDWIDTH 20

using std::vector;

class State {
   public:
      //variable
      int cost;
      float alpha;

      //function
      State();
      State(const State& sourceState); //copy constructor
      State& operator=(const State& sourceState); 
      ~State();

      void init();
      void generateNewState(RandomGenerator random);
      int isLegal();
      void printState();
      
      void printAddr();//for dubug only
   private:
      //all links assume to have same latency and bandwidth
      //int LINK_LATENCY;
      //int LINK_BANDWIDTH;

      //variable
      int **bandwidth;
      int **latency;
      int meshSize;

      int numCore;//total number of core
      vector<Core> core; 

      //function
      void calculateCost();
      int compactionCost();
      int dilationCost();
      int getHops(Coordinate a, Coordinate b);
};
