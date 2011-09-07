#ifndef COST_HPP
#define COST_HPP

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
      void initCost(double** bandwidth, double** latency, vector<Core> core, const double LINK_LATENCY, Network& network);
      double getCost() const;
      void printCost() const;
      void printSummary() const;

      void calculateCost(double** bandwidth, vector<Core> core, Network& network);
      void updateCost(double** bandwidth, double** latency, double LINK_LATENCY, vector<Core> core, int index, int op);

   private:
      //variable
      double alpha, beta, gamma, delta;
      double cost;
      double compaction, dilation, slack, proximity, utilization;

      //function
      double initCompaction(double** bandwidth, vector<Core> core);
      double initDilation(double** bandwidth, double** latency, vector<Core> core, const double LINK_LATENCY, Network& network);
      double initSlack(double** latency, vector<Core> core, const double LINK_LATENCY);
      double initProximity(double** bandwidth, vector<Core> core);
      double utilizationCost(double** bandwidth, vector<Core> core, Network& network);

      //calculate connection from/to one core (index) only
      double changeCompaction(double** bandwidth, vector<Core> core, int index);
      double changeSlack(double** latency, vector<Core> core, const double LINK_LATENCY, int index);
      double changeProximity(double** bandwidth, vector<Core> core, int index);
};

#endif
