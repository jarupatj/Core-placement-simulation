#ifndef SA_HPP
#define SA_HPP

#include "State.hpp"

class SimulatedAnnealing {
   public:
      SimulatedAnnealing();
      ~SimulatedAnnealing();

      /*
       * Initialize simulated annealing
       */
      int init(double alpha, double beta, double gamma, double delta, \
               double startTemp, double endTemp, double rate, int iter, \
               int reject, int accept, char* inputfile, bool verbose, bool quiet );
      /*
       * starts simulated annealing
       */
      void run();
      /*
       * print summary of the final best state
       * for verbose and normal printing
       */
      void printSummary() const;
      /*
       * print table headings
       */
      void initTable() const;
      /*
       * print cost summary for quiet printing
       */
      void printFinalCost() const;

   private:
      //constant
      int MAX_STATE_CHANGE_PER_TEMP;
      int MAX_REJECT;
      int MAX_ACCEPT;
      double TEMP_CHANGE_FACTOR;
      double END_TEMP;

      //variable
      State currentState;
      State bestState;
      double temp;
      double bestTemp; //temp that achieve best configuration
      bool verbose;
      bool quiet;

      //function
      int getCost();

      //void printState(const State& state) const;
      void printState(const State& state, int& iterations, \
                      const char& newStateFlag = 'Y', \
                      const double& randomNum = -1) const;
};

#endif
