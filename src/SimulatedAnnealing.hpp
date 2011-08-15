#ifndef SA_HPP
#define SA_HPP

#include "State.hpp"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      int init(double alpha, double beta, double gamma, double theta, \
               double startTemp, double endTemp, double rate, int iter, \
               char* inputfile, bool verbose );
      void run();
      void printSummary();
      void initTable();

   private:
      //constant
      int MAX_STATE_CHANGE_PER_TEMP;
      double TEMP_CHANGE_FACTOR;
      double END_TEMP;

      //variable
      State currentState;
      State bestState;
      double temp;
      double bestTemp; //temp that achieve best config
      bool verbose;

      //function
      int getCost();

      void printState(const State& state);
      void printStateVerbose(const State& state, const char& newStateFlag, const double& randomNum);
};

#endif
