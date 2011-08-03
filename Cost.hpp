#pragma once

#include <vector>

#include "Network.hpp"
#include "Core.hpp"

using std::vector;

class Cost {
   public:
      Cost();
      ~Cost();

      //function
      void init(double alpha, double beta, double gamma, double theta);
      void initCost(int** bandwidth, int** latency, vector<Core> core, const int LINK_LATENCY, Network& network); 
      double getCost();
      void printCost();

   private:
      //variable
      double alpha, beta, gamma, theta;
      double cost;
      double compaction, dilation, slack, proximity, utilization;

      //function
      double compactionCost(int** bandwidth, vector<Core> core);
      double dilationCost(int** bandwidth, int** latency, vector<Core> core, const int LINK_LATENCY, Network& network); 
      double slackCost(int** latency, vector<Core> core, const int LINK_LATENCY); 
      double proximityCost(int** bandwidth, vector<Core> core); 
      double utilizationCost(int** bandwidth, vector<Core> core, Network& network); 
      int getHops(Coordinate a, Coordinate b);

};
