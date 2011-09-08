#ifndef COST_HPP
#define COST_HPP

#include <vector>
#include <cmath>
#include <string>

#include "Defs.hpp"
#include "Network.hpp"
#include "Core.hpp"

using std::vector;
using std::string;

class Cost {
   public:
      Cost();
      ~Cost();

      //functions
      /*
       * Initialize alpha, beta, gamma and delta
       */
      void init(double alpha, double beta, double gamma, double delta);
      /*
       * Initialize cost of a state
       */
      void initCost(double** bandwidth, double** latency, vector<Core> core, const double LINK_LATENCY, Network& network);
      /*
       * Get cost
       */
      double getCost() const;
      /*
       * print cost in a format for verbose and normal printing
       */
      void printCost() const;
      /*
       * print cost in a format for quiet print
       */
      void printSummary() const;

      /*
       * calculate cost when compation, slack and proximity are initialized
       */
      void calculateCost(double** bandwidth, vector<Core> core, Network& network);
      /*
       * Add/remove compaction, slack and proximity cost when a core is moved.
       * index specifies indexing number to access moved core in core vector
       * op specifies operation ADD or REMOVE
       */
      void updateCost(double** bandwidth, double** latency, double LINK_LATENCY, vector<Core> core, int index, int op);

      string printQuiet() const;

   private:
      //variable
      double alpha, beta, gamma, delta;
      double cost;
      double compaction, dilation, slack, proximity, utilization;

      //function
      /*
       * initialize compaction, dilation, slack and proximity cost
       */
      double initCompaction(double** bandwidth, vector<Core> core);
      double initDilation(double** bandwidth, double** latency, vector<Core> core, const double LINK_LATENCY, Network& network);
      double initSlack(double** latency, vector<Core> core, const double LINK_LATENCY);
      double initProximity(double** bandwidth, vector<Core> core);

      /*
       * Update and calculate utilization cost
       */
      double utilizationCost(double** bandwidth, vector<Core> core, Network& network);

      /*
       * calculate change in compation, slack and proximity cost
       * the change is calculate when core[index] is moved
       * index specifies indexing number to access moved core in core vector
       * the change in cost is calculated using connection from/to core[index]
       */
      double changeCompaction(double** bandwidth, vector<Core> core, int index);
      double changeSlack(double** latency, const double LINK_LATENCY, vector<Core> core, int index);
      double changeProximity(double** bandwidth, vector<Core> core, int index);
};

#endif
