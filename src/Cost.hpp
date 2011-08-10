#ifndef COST_H
#define COST_H

#include <vector>
#include <cmath>

#include "Defs.hpp"
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
      void printSummary();

      void calculateCost(int** bandwidth, vector<Core> core, Network& network); 
      void updateCost(int** bandwidth, int** latency, int LINK_LATENCY, vector<Core> core, int index, int op); 

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

      //calculate connection from/to one core (index) only
      double compactionCost(int** bandwidth, vector<Core> core, int index);
      double slackCost(int** latency, vector<Core> core, const int LINK_LATENCY, int index); 
      double proximityCost(int** bandwidth, vector<Core> core, int index); 
};

inline int Cost::getHops(Coordinate a, Coordinate b) {
   return fabs(a.x - b.x) + fabs(a.y - b.y);
}

#endif
