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
      float alpha, beta, gamma, theta;

      //function
      State();
      State(const State& sourceState); //copy constructor
      State& operator=(const State& sourceState); 
      ~State();

      int init(char* filename, RandomGenerator random);
      void generateNewState(RandomGenerator random);
      bool isLegal();
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
      int compaction, dilation, slack, proximity, utilization;

      int numCore;//total number of core
      vector<Core> core; 

      Network network;

      //function
      void deepCopy(const State& sourceState);
      void calculateCost();
      int compactionCost();
      int dilationCost();
      int slackCost();
      int proximityCost();
      int utilizationCost();
      int getHops(Coordinate a, Coordinate b);
};
