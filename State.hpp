#pragma once

#include <vector>

#include "Core.hpp"
#include "RandomGenerator.hpp"
#include "Network.hpp"

//all links assume to have same latency and bandwidth
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
      //variable
      int **bandwidth;
      int **latency;
      int meshRow;
      int meshCol;

      int numCore;//total number of core
      vector<Core> core; 

      Network network;

      //function
      void calculateCost();
      int compactionCost();
      int dilationCost();
      int getHops(Coordinate a, Coordinate b);
};
