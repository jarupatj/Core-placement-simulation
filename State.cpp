#include <iostream>
#include <cmath>

#include "State.hpp"

using namespace std;

State::State() {
   cost = 0;
   alpha = 1;
   bandwidth = NULL;
   latency = NULL;
   meshRow= 0;
   meshCol= 0;
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
   alpha = sourceState.alpha;
   meshRow= sourceState.meshRow;
   meshCol= sourceState.meshCol;
   numCore = sourceState.numCore;
   core = sourceState.core;
   network = sourceState.network;

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
   alpha = sourceState.alpha;
   meshRow= sourceState.meshRow;
   meshCol= sourceState.meshCol;
   numCore = sourceState.numCore;
   core = sourceState.core;
   network = sourceState.network;

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
   meshRow= 4;
   meshCol= 4;
   numCore = 5;
   network.init(meshRow, meshCol);
   
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

   bandwidth[3][0] = 10;
   latency[3][0] = 10;

   bandwidth[3][4] = 10;
   latency[3][4] = 10;

   bandwidth[4][1] = 20;
   latency[4][1] = 10;

   core.push_back(Core(0, 0));
   core.push_back(Core(3, 3));
   core.push_back(Core(2, 1));
   core.push_back(Core(0, 3));
   core.push_back(Core(1, 3));

   //set router in the network
   Coordinate pos = {0,0};
   network.addCore(pos, 0);
   pos.x = 3;
   pos.y = 3;
   network.addCore(pos, 1);
   pos.x = 2;
   pos.y = 1;
   network.addCore(pos, 2);
   pos.x = 0;
   pos.y = 3;
   network.addCore(pos, 3);
   pos.x = 1;
   pos.y = 3;
   network.addCore(pos, 4);
   //calculate initial cost
   calculateCost();

}

void State::calculateCost() {
   cost = alpha * compactionCost() + (1-alpha) * dilationCost();
}

int State::compactionCost() {
   int sum = 0;
   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         if(bandwidth[i][j] != 0) {
            sum += bandwidth[i][j] * getHops(core[i].getPosition(),core[j].getPosition());
         }
      }
   }
   return sum;
}

int State::getHops(Coordinate a, Coordinate b) {
   return fabs(a.x - b.x) + fabs(a.y - b.y);
}

int State::dilationCost() {
   return 0;
}

int State::isLegal() {
   int hops;
   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         if(latency[i][j] != 0) {
            hops = getHops(core[i].getPosition(), core[j].getPosition());
            if(latency[i][j] < hops * LINK_LATENCY) {
               return 0;
            }
         }
      }
   }
   return 1;
}

void State::generateNewState(RandomGenerator random) {
   //randomly select one core
   int changedCore = random.uniform_n(numCore);
   //randomly select new position
   Coordinate newPos;
   newPos.x = random.uniform_n(meshCol);
   newPos.y = random.uniform_n(meshRow);
   
   //cout << "c core: " << changedCore << " newPos: " << newPos.x << "," << newPos.y << endl;

   //check if that pos is empty or not
   if( network.hasCore(newPos) ) {
      //remove old cost
      //swap
      Coordinate oldPos = core[changedCore].getPosition();
      int swapIndex = network.getCoreIndex(newPos);
      //place core on new pos
      core[swapIndex].setPosition(oldPos);
      core[changedCore].setPosition(newPos);
      network.addCore(oldPos, swapIndex);
      network.addCore(newPos, changedCore);
   } else {
      //remove old cost

      //move core from old pos
      network.removeCore(core[changedCore].getPosition());
      //place core on new pos
      core[changedCore].setPosition(newPos);
      network.addCore(core[changedCore].getPosition(), changedCore);
   }
   //calculate new cost
   calculateCost();
}

void State::printState() {
   cout << "current state: " << endl;
   for(unsigned int i = 0; i < core.size(); i++) {
      core[i].printCore();
   }
   cout << "cost: " << cost << endl;
}

void State::printAddr() {
   cout << "b/w : " << bandwidth << endl;
   cout << "laten : " << latency << endl;
   cout << "meshRow: " << meshRow<< endl;
   cout << "meshCol: " << meshCol<< endl;
   for(unsigned int i = 0; i < core.size(); i++) {
      core[i].printCore();
      cout << &core[i] << endl;
   }
}
