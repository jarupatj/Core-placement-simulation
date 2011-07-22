#include "State.h"
#include <iostream>

using namespace std;

State::State() {
   cost = 0;
   bandwidth = NULL;
   latency = NULL;
   meshSize = 0;
   numCore = 0;
}

State::~State(){
   for(int i = 0; i < numCore; i++) {
      delete [] bandwidth[i];
      delete [] latency[i];
   }

   delete bandwidth;
   delete latency;
}

State::State(const State& sourceState) {
   cost = sourceState.cost;
   meshSize = sourceState.meshSize;
   numCore = sourceState.numCore;
   core = sourceState.core;

   if(sourceState.bandwidth) {
      //allocate memory
      bandwidth = new int* [numCore];
      for(int i = 0; i < numCore; i++) {
         bandwidth[i] = new int [numCore];
      }
      //copy 
      for(int i = 0; i < numCore; i++) {
         for(int j = 0; j < numCore; j++) {
            bandwidth[i][j] = sourceState.bandwidth[i][j];
         }
      }
   } else {
      bandwidth = 0;
   }
   
   if(sourceState.latency) {
      //allocate memory
      latency = new int* [numCore];
      for(int i = 0; i < numCore; i++) {
         latency[i] = new int [numCore];
      }
      //copy 
      for(int i = 0; i < numCore; i++) {
         for(int j = 0; j < numCore; j++) {
            latency[i][j] = sourceState.latency[i][j];
         }
      }
   } else {
      latency = 0;
   }

   if(!sourceState.core.empty()) {
   }
}

State& State::operator=(const State& sourceState) {
   //check for self-assignment (same obj)
   if (this == &sourceState) {
      return *this;
   }

   for(int i = 0; i < numCore; i++) {
      delete [] bandwidth[i];
      delete [] latency[i];
   }

   delete bandwidth;
   delete latency;

   core.clear();

   cost = sourceState.cost;
   meshSize = sourceState.meshSize;
   numCore = sourceState.numCore;
   core = sourceState.core;

   if(sourceState.bandwidth) {
      //allocate memory
      bandwidth = new int* [numCore];
      for(int i = 0; i < numCore; i++) {
         bandwidth[i] = new int [numCore];
      }
      //copy 
      for(int i = 0; i < numCore; i++) {
         for(int j = 0; j < numCore; j++) {
            bandwidth[i][j] = sourceState.bandwidth[i][j];
         }
      }
   } else {
      bandwidth = 0;
   }
   
   if(sourceState.latency) {
      //allocate memory
      latency = new int* [numCore];
      for(int i = 0; i < numCore; i++) {
         latency[i] = new int [numCore];
      }
      //copy 
      for(int i = 0; i < numCore; i++) {
         for(int j = 0; j < numCore; j++) {
            latency[i][j] = sourceState.latency[i][j];
         }
      }
   } else {
      latency = 0;
   }

   return *this;
}

void State::init(){
   meshSize = 4;
   numCore = 4;
   
   bandwidth = new int* [numCore];
   latency = new int* [numCore];
   for(int i = 0; i < numCore; i++) {
      bandwidth[i] = new int [numCore];
      latency[i] =  new int [numCore];
   }

   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         bandwidth[i][j] = 0;
         latency[i][j] = 0;
      }
   }

   bandwidth[0][1] = 20;
   latency[0][1] = 10;

   bandwidth[1][2] = 30;
   latency[1][2] = 20;

   bandwidth[2][3] = 40;
   latency[2][3] = 20;

   bandwidth[3][0] = 20;
   latency[3][0] = 10;

   core.push_back(Core(0, 0));
   core.push_back(Core(3, 3));
   core.push_back(Core(2, 1));
   core.push_back(Core(0, 3));

   //calculate initial cost
   //calculateCost();

}

void State::printAddr() {
   cout << "b/w : " << bandwidth << endl;
   cout << "laten : " << latency << endl;
   cout << "meshSize: " << meshSize<< endl;
   cout << "core: " << &core[0] << endl;
}

void State::generateNewState() {
}

