#ifndef STATE_HPP
#define STATE_HPP

#include <utility>
#include <vector>
#include <cmath>
#include <string>

#include "Defs.hpp"
#include "Core.hpp"
#include "Network.hpp"
#include "Cost.hpp"

using std::vector;
using std::pair;
using std::string;

class State {
   public:
      //function
      State();
      ~State();

      /*
       * Initialize a state
       */
      int init(double alpha, double beta, double gamma, double theta, \
               char* filename);
      /*
       * generate new state from current state
       */
      void generateNewState();
      /*
       * check if the state is legal
       * - using latency constraint
       */
      bool isLegal();
      /*
       * get current cost of a state
       */
      double getCost() const;
      /*
       * print cost details of a state
       */
      void printState() const;
      /*
       * print state summary
       * - cost
       * - diagram
       * - max bandwidth in a link
       */
      void printSummary() const;
      /*
       * Generate output in an input format
       * so that it can be used as input for simulator
       */
      void generateOutput(char* fileName);
      /*
       * print a list of illegal connections
       */
      void printIllegalConnection();
      /*
       * Return a string which consists of cost value
       * This function returns a string instead of printing because
       * we will used this function to send a result in MPI job
       */
      string printQuiet() const;
      /*
       * print latency table
       */
      void printLatencyTable();
   private:
      double LINK_BANDWIDTH;
      double LINK_LATENCY;
      
      //variable
      double bandwidth[MAX_CORE_SIZE][MAX_CORE_SIZE];
      double latency[MAX_CORE_SIZE][MAX_CORE_SIZE];
      int meshRow;
      int meshCol;

      vector<Core> core; 
      Network network;
      Cost cost;

      /*
       * List of illegal conneciton pair of "from" and "to"
       */
      vector< pair <unsigned int,unsigned int> > illegalConnection;

      //function
      void deepCopy(const State& sourceState);
};

#endif
