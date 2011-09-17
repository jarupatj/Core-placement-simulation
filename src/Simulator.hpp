#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <sstream>

#include "State.hpp"

using std::stringstream;

class Simulator {
   public:
      Simulator();
      ~Simulator();

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
       * Generate output in an input format
       * so that it can be used as input for simulator
       */
      void generateOutput(char* fileName);
      /*
       * print a list of illegal connections
       */
      void printIllegalConnection();
      /*
       * print cost summary for quiet printing
       * Return a string which consists of cost value
       * This function returns a string instead of printing because
       * we will used this function to send a result in MPI job
       */
      string printFinalCost() const;
      /*
       * print latency table
       */
      void printLatencyTable();

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
      /*
       * get current cost of a state
       */
      int getCost();
      /*
       * print a state detail in tabular format
       * used for verbose and normal printing
       */
      void printState(const State& state, int& iterations, \
                      const char& newStateFlag = 'Y', \
                      const double& randomNum = -1) const;
};

#endif
