#pragma once

#include <vector>

#include "Core.hpp"
#include "RandomGenerator.hpp"
#include "Network.hpp"

//all links assume to have same latency and bandwidth
//#define LINK_LATENCY 10
//#define LINK_BANDWIDTH 40

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

      int init(char* filename, RandomGenerator random);
      void generateNewState(RandomGenerator random);
      int isLegal();
      void printState();
      
      void printAddr();//for dubug only
   private:
      int LINK_BANDWIDTH;
      int LINK_LATENCY;
      
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
