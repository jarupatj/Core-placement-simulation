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
      void initCost(vector< vector<double> > &bandwidth, vector< vector<double> > &latency , vector<Core> &core, const double LINK_LATENCY, Network& network);
      /*
       * Get cost
       */
      double getCost() const;
      /*
       * print cost in a tabular format for verbose and normal printing
       */
      void printCost() const;
      /*
       * print cost in a detail cost in summary format
       * for verbose and normal printing
       */
      void printSummary() const;

      /*
       * calculate cost when compation, slack and proximity are initialized
       */
      void calculateCost(vector< vector<double> > &bandwidth, vector<Core> &core, Network& network);
      /*
       * Add/remove compaction, slack and proximity cost when a core is moved.
       * index specifies indexing number to access moved core in core vector
       * op specifies operation ADD or REMOVE
       */
      void updateCost(vector< vector<double> > &bandwidth, vector< vector<double> > &latency, double LINK_LATENCY,
            vector<Core> &core, int op, int coreA, int coreB = NO_CORE);
      /*
       * Return a string which consists of cost value
       * This function returns a string instead of printing because
       * we will used this function to send a result in MPI job
       */
      string printQuiet() const;

      double getCostRatio();

   private:
      //variable
      double alpha, beta, gamma, delta;
      double cost, initialCost;
      double compaction, dilation, slack, proximity, utilization;

      //function
      /*
       * initialize compaction, dilation, slack and proximity cost
       */
      double initCompaction(vector< vector<double> > &bandwidth, vector<Core> &core);
      double initDilation(vector< vector<double> > &bandwidth, vector< vector<double> > &latency, vector<Core> &core, const double LINK_LATENCY, Network& network);
      double initSlack(vector< vector<double> > &latency, vector<Core> &core, const double LINK_LATENCY);
      double initProximity(vector< vector<double> > &bandwidth, vector<Core> &core);

      /*
       * Update and calculate utilization cost
       */
      double utilizationCost(vector< vector<double> > &bandwidth, vector<Core> &core, Network& network);


      /*
       * calculate change in compation, slack and proximity cost
       * the change is calculate when core[coreA] is moved
       * or core[coreA] is swapped with core[coreB]
       * coreA, coreB specify indexing number to access moved core in core vector
       * the change in cost is calculated using connection from/to core[coreA] and core[coreB]
       */
      double changeCompaction(vector< vector<double> > &bandwidth, vector<Core> &core, int coreA, int coreB);
      double changeSlack(vector< vector<double> > &latency, const double LINK_LATENCY, vector<Core> &core, int coreA, int coreB);
      double changeProximity(vector< vector<double> > &bandwidth, vector<Core> &core, int coreA, int coreB);
};

#endif
