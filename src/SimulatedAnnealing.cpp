#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "SimulatedAnnealing.hpp"
#include "Utils.hpp"

using namespace std;

SimulatedAnnealing::SimulatedAnnealing() {}

SimulatedAnnealing::~SimulatedAnnealing() {}

int SimulatedAnnealing::init(double alpha, double beta, double gamma, double theta, \
                             double startTemp, double endTemp, double rate, int iter, \
                             char* inputfile, bool verbose ) {
   temp = startTemp; 
   END_TEMP = endTemp; 
   TEMP_CHANGE_FACTOR = rate; 
   MAX_STATE_CHANGE_PER_TEMP = iter; 
   this->verbose = verbose;

   int err = currentState.init(alpha, beta, gamma, theta, inputfile);
   if( err != 0 ) {
      return err;
   }
   bestState = currentState;
   return 0;
}

void SimulatedAnnealing::run() {
   int changeCost;
   bool setCurrent = false;
   double random, prob;

   while( temp > END_TEMP ) {
      for(int numChange = 0; numChange < MAX_STATE_CHANGE_PER_TEMP; numChange++) {
         State newState(currentState); //deep copy
         newState.generateNewState();
         changeCost = newState.getCost()- currentState.getCost();

         //check legality
         if( newState.isLegal() ) {
            if( changeCost < 0 ) {
               setCurrent = true;
               if(verbose) {
                  printStateVerbose(newState, 'Y', -1);
               }
            } else {
               random = uniform_0_1();
               prob = exp( -changeCost / temp );
               if( random < prob) {
                  setCurrent = true;
                  if(verbose) {
                     printStateVerbose(newState, 'Y', random);
                  }
               } else {
                  if(verbose) {
                     printStateVerbose(currentState, ' ', random);
                  }
               }
            }
         } else {
            if(verbose) {
               printStateVerbose(currentState, ' ', -1);
            }
         }

         if( setCurrent ) {
            setCurrent = false;
            currentState = newState; //deep copy
            if( newState.isLegal() && currentState.getCost() <= bestState.getCost()) {
               bestState = currentState;
               bestTemp = temp;
            }
         } 
      }

      if(!verbose) {
         printState(bestState);
      }
      temp = temp * TEMP_CHANGE_FACTOR;
   }
}

void SimulatedAnnealing::initTable() {
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

void SimulatedAnnealing::printState(const State& state) {
   cout << setw(10) << setiosflags(ios::fixed) << setprecision(3) << temp;
   state.printState();
   cout << endl;
}

void SimulatedAnnealing::printStateVerbose(const State& state, const char& newStateFlag, const double& randomNum) {
   cout << setw(10) << setiosflags(ios::fixed) << setprecision(3) << temp;
   state.printState();
   cout << setw(12) << newStateFlag
   << setw(12) << randomNum
   << endl;
}

void SimulatedAnnealing::printSummary() {
   cout << "Temperature achieve: " << bestTemp << endl;
   bestState.printSummary();
}
