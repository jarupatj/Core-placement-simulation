#ifndef STATE_HPP
#define STATE_HPP

#include <utility>
#include <vector>
#include <cmath>

#include "Defs.hpp"
#include "Core.hpp"
#include "Network.hpp"
#include "Cost.hpp"

using std::vector;
using std::pair;

class State {
   public:
      //function
      State();
      State(const State& sourceState); //copy constructor
      State& operator=(const State& sourceState); 
      ~State();

      int init(double alpha, double beta, double gamma, double theta, \
               char* filename);
      void generateNewState();
      bool isLegal();
      double getCost();
      void printState() const;
      void printSummary() const;
      
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

      vector< pair <unsigned int,unsigned int> > illegalConnection;

      //function
      void deepCopy(const State& sourceState);
};

#endif
