#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "SimulatedAnnealing.hpp"
#include "Utils.hpp"

using namespace std;

SimulatedAnnealing::SimulatedAnnealing() {}

SimulatedAnnealing::~SimulatedAnnealing() {}

int SimulatedAnnealing::init(double alpha, double beta, double gamma, double delta, \
                             double startTemp, double endTemp, double rate, int iter, \
                             int reject, char* inputfile, bool verbose ) {
   temp = startTemp; 
   END_TEMP = endTemp; 
   TEMP_CHANGE_FACTOR = rate; 
   MAX_STATE_CHANGE_PER_TEMP = iter;
   MAX_REJECT = reject;
   this->verbose = verbose;

   int err = currentState.init(alpha, beta, gamma, delta, inputfile);
   if( err != 0 ) {
      return err;
   }
   bestState = currentState;
   return 0;
}

void SimulatedAnnealing::run() {
   int changeCost, cReject;
   bool setCurrent = false;
   double random, prob;

   while( temp > END_TEMP ) {
      cReject = 0;
      for(int numChange = 0; (numChange < MAX_STATE_CHANGE_PER_TEMP) && (cReject < MAX_REJECT); numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState();
         changeCost = newState.getCost()- currentState.getCost();

         /*
          * Check current state legality
          */
         if( newState.isLegal() ) {
            /*
             * Always accept lower cost state
             */
            if( changeCost < 0 ) {
               setCurrent = true;
               if(verbose) printStateVerbose(newState, 'Y', -1);
            } else {
               random = uniform_0_1();
               prob = exp( -changeCost / temp );
               /*
                * Accept higher cost with probability
                */
               if( random < prob) {
                  setCurrent = true;
                  if(verbose) printStateVerbose(newState, 'Y', random);
               } else {
                  cReject++;
                  if(verbose) printStateVerbose(currentState, ' ', random);
               }
            }
         } else {
            cReject++;
            if(verbose) printStateVerbose(currentState, ' ', -1);
         }

         /*
          * Set new state to currentState
          */
         if( setCurrent ) {
            setCurrent = false;
            cReject = 0;
            currentState = newState;
            /*
             * Keep track of best state so far
             */
            if( newState.isLegal() && currentState.getCost() < bestState.getCost()) {
               bestState = currentState;
               bestTemp = temp;
            }
         } 
      }

      if(!verbose) printState(bestState);
      temp = temp * TEMP_CHANGE_FACTOR;
   }
}

void SimulatedAnnealing::initTable() const {
   cout << setw(10) << "Temp"
   << setw(12) << "Cost"
   << setw(12) << "Compaction"
   << setw(12) << "Dilation"
   << setw(12) << "Slack"
   << setw(12) << "Proximity"
   << setw(12) << "Util"
   << setw(12) << "New state"
   << setw(12) << "Random #"
   << endl;
}

void SimulatedAnnealing::printState(const State& state) const {
   cout << setw(10) << setiosflags(ios::fixed) << setprecision(3) << temp;
   state.printState();
   cout << endl;
}

void SimulatedAnnealing::printStateVerbose(const State& state, const char& newStateFlag, \
                                           const double& randomNum) const {
   ostringstream strs;
   strs << randomNum;
   string str = (randomNum == -1) ? " " : strs.str();

   cout << setw(10) << setiosflags(ios::fixed) << setprecision(3) << temp;
   state.printState();
   cout << setw(12) << newStateFlag
   << setw(12) << str
   << endl;
}

void SimulatedAnnealing::printSummary() const {
   cout << "Temperature achieve: " << bestTemp << endl;
   bestState.printSummary();
}
