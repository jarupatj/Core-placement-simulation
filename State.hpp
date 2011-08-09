#pragma once

#include <vector>
#include <cmath>

#include "Core.hpp"
#include "RandomGenerator.hpp"
#include "Network.hpp"
#include "Cost.hpp"

//all links assume to have same latency and bandwidth
//#define LINK_LATENCY 10
//#define LINK_BANDWIDTH 40

using std::vector;

class State {
   public:
      //variable
      //double cost;
      double alpha, beta, gamma, theta;

      //function
      State();
      State(const State& sourceState); //copy constructor
      State& operator=(const State& sourceState); 
      ~State();

      int init(char* filename, RandomGenerator random);
      void generateNewState(RandomGenerator random);
      bool isLegal();
      double getCost();
      void printState();
      
   private:
      int LINK_BANDWIDTH;
      int LINK_LATENCY;
      
      //variable
      int **bandwidth;
      int **latency;
      int meshRow;
      int meshCol;

      vector<Core> core; 
      Network network;
      Cost cost;

      //function
      void deepCopy(const State& sourceState);
      int getHops(Coordinate a, Coordinate b);
};


inline int State::getHops(Coordinate a, Coordinate b) {
   return fabs(a.x - b.x) + fabs(a.y - b.y);
}
