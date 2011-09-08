#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "SimulatedAnnealing.hpp"
#include "Utils.hpp"

using namespace std;

SimulatedAnnealing::SimulatedAnnealing() {
}

SimulatedAnnealing::~SimulatedAnnealing() {
}

int SimulatedAnnealing::init(double alpha, double beta, double gamma,
      double delta, double startTemp, double endTemp, double rate, int iter,
      int reject, int accept, char* inputfile, bool verbose, bool quiet) {
   temp = startTemp;
   bestTemp = startTemp;
   END_TEMP = endTemp;
   TEMP_CHANGE_FACTOR = rate;
   MAX_STATE_CHANGE_PER_TEMP = iter;
   MAX_REJECT = reject;
   MAX_ACCEPT = accept;
   this->verbose = verbose;
   this->quiet = quiet;

   /*
    * Initialize intial state
    */
   int err = currentState.init(alpha, beta, gamma, delta, inputfile);
   if (err != 0) {
      return err;
   }
   bestState = currentState;
   return 0;
}

void SimulatedAnnealing::run() {
   int changeCost, cReject, cAccept, iterations;
   bool setCurrent = false;
   double random, prob;

   iterations = 0;
   while (temp > END_TEMP) {
      cReject = 0;
      cAccept = 0;
      /*
       * Change temperature when one of the condition is met
       * - attempt to change state more than MAX_STATE_CHANGE_PER_TEMP
       * - has consecutive number of state rejection equals MAX_REJECT
       * - has accepted new state for MAX_ACCEPT
       */
      for (int numChange = 0; (numChange < MAX_STATE_CHANGE_PER_TEMP)
            && (cReject < MAX_REJECT) && (cAccept < MAX_ACCEPT); numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState();
         changeCost = newState.getCost() - currentState.getCost();

         iterations++;

         /*
          * Check current state legality
          */
         if (newState.isLegal()) {
            /*
             * Always accept lower cost state
             */
            if (changeCost < 0) {
               setCurrent = true;
               if (verbose)
                  printState(newState, iterations, 'Y', -1);
            } else {
               random = uniform_0_1();
               prob = exp(-changeCost / temp);
               /*
                * Accept higher cost with probability
                */
               if (random < prob) {
                  setCurrent = true;
                  if (verbose)
                     printState(newState, iterations, 'Y', random);
               } else {
                  cReject++;
                  if (verbose)
                     printState(newState, iterations, ' ', random);
               }
            }
         } else {
            cReject++;
            if (verbose)
               printState(currentState, iterations, ' ', -1);
         }

         /*
          * Set new state to currentState
          */
         if (setCurrent) {
            setCurrent = false;
            cReject = 0;
            cAccept++;
            currentState = newState;
            /*
             * Keep track of best state so far
             */
            if (newState.isLegal() && currentState.getCost()
                  < bestState.getCost()) {
               bestState = currentState;
               bestTemp = temp;
            }
         }
      }

      if (!verbose && !quiet) {
         printState(bestState, iterations);
      }
      temp = temp * TEMP_CHANGE_FACTOR;
   }
}

void SimulatedAnnealing::initTable() const {
   cout << "#" << setw(11) << "Iterations" << setw(12) << "Temp" << setw(12)
         << "Cost" << setw(12) << "Compaction" << setw(12) << "Dilation"
         << setw(12) << "Slack" << setw(12) << "Proximity" << setw(12)
         << "Util" << setw(12) << "New state" << setw(12) << "Random #" << endl;
}

void SimulatedAnnealing::printState(const State& state, int& iterations,
      const char& newStateFlag, const double& randomNum) const {

   /*
    * comment out the line (for gnuplot)
    * if that line does not correspond to new state
    */
   cout << (newStateFlag == 'Y' ? " " : "#");
   cout << setw(11) << iterations;
   cout << setw(12) /*<< setiosflags(ios::fixed)*/<< setprecision(6) << temp;

   /*
    * print detail cost of a state
    */
   state.printState();

   /*
    * in verbose printing
    * print random number and newStateFlag
    */
   if (verbose) {
      ostringstream strs;
      strs << setiosflags(ios::fixed) << setprecision(6) << randomNum;
      string str = (randomNum == -1) ? " " : strs.str();
      cout << setw(12) << newStateFlag << setw(12) << str;
   }

   cout << endl;
}

void SimulatedAnnealing::printSummary() const {
   cout << "# Temperature achieve: " << setprecision(6) << bestTemp << endl;
   bestState.printSummary();
}

void SimulatedAnnealing::printFinalCost() const {
   cout << setiosflags(ios::fixed) << setprecision(3);
   bestState.printQuiet();
   cout << endl;
}

void SimulatedAnnealing::generateOutput(char* fileName) {
   bestState.generateOutput(fileName);
}
