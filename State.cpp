#include <iostream>
#include <cmath>
#include <cstdio>

#include "State.hpp"

using namespace std;

State::State() {
   alpha = 1;
   beta = 1;
   gamma = 0.2;
   theta = 0.04;
   bandwidth = NULL;
   latency = NULL;
   meshRow= 0;
   meshCol= 0;
   numCore = 0;
   compaction = 0;
   dilation = 0;
   slack = 0;
   proximity = 0;
   utilization = 0;
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
   deepCopy(sourceState);
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
   
   deepCopy(sourceState);

   return *this;
}

void State::deepCopy(const State& sourceState) {
   LINK_BANDWIDTH = sourceState.LINK_BANDWIDTH;
   LINK_LATENCY = sourceState.LINK_LATENCY;
   cost = sourceState.cost;
   alpha = sourceState.alpha;
   beta = sourceState.beta;
   gamma = sourceState.gamma;
   theta = sourceState.theta;

   meshRow= sourceState.meshRow;
   meshCol= sourceState.meshCol;
   numCore = sourceState.numCore;
   compaction = sourceState.compaction;
   dilation = sourceState.dilation;
   slack = sourceState.slack;
   proximity = sourceState.proximity;
   utilization = sourceState.utilization;

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
      bandwidth = NULL;
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
      latency = NULL;
   }
}

int State::init(char* filename, RandomGenerator random){
   FILE *fp = fopen(filename, "r");
   if(fp == NULL) {
      cout << "error can't open inputfile" << endl;
      return 1;
   }
   fscanf(fp, "%d%d", &LINK_BANDWIDTH, &LINK_LATENCY);
   cout << "link bw/laten: " << LINK_BANDWIDTH << " " << LINK_LATENCY<< endl;

   fscanf(fp, "%d%d", &meshRow, &meshCol);
   cout << "meshRow/Col: " << meshRow << " " << meshCol << endl;
   fscanf(fp, "%d", &numCore);
   cout << "numCore: " << numCore << endl;

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

   int x,y;
   network.init(meshRow, meshCol);
   for(int i = 0; i < numCore; i++) {
      fscanf(fp, "%d%d", &x, &y);
      core.push_back(Core(x,y));
      network.addCore(core[i].getPosition(), i);
   }
   
   int from, to, bw, laten;
   while( !feof(fp) ) {
      fscanf(fp, "%d%d%d%d", &from, &to, &bw, &laten);
      bandwidth[from-1][to-1] = bw;
      latency[from-1][to-1] = laten;
   }
   fclose(fp);

   //add connections
   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         //has a connection from i to j
         if(bandwidth[i][j] != 0) {
            network.addConnection(core[i].getPosition(), core[j].getPosition());
         }
      }
   }

   if( !isLegal() ) {
      std::cout << "illegal initial position\n";
      return 1;
   }
   
   //calculate initial cost
   cost.init(alpha, beta, gamma, theta);
   cost.initCost(bandwidth, latency, core, LINK_LATENCY, network);

   return 0;
}

double State::getCost() {
   return cost.getCost();
}

int State::getHops(Coordinate a, Coordinate b) {
   return fabs(a.x - b.x) + fabs(a.y - b.y);
}

bool State::isLegal() {
   int hops;
   //check latency legality
   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         if(latency[i][j] != 0) {
            hops = getHops(core[i].getPosition(), core[j].getPosition());
            if(latency[i][j] < hops * LINK_LATENCY) {
               return false;
            }
         }
      }
   }
   return true;
}

void State::generateNewState(RandomGenerator random) {
   //randomly select one core
   int changedCore = random.uniform_n(numCore);
   //randomly select new position
   Coordinate newPos;
   newPos.x = random.uniform_n(meshCol);
   newPos.y = random.uniform_n(meshRow);
   
   //if the new position is not empty 
   if( network.hasCore(newPos) ) {
      //swap
      Coordinate oldPos = core[changedCore].getPosition();
      int swapCore = network.getCoreIndex(newPos);
      //remove all connections from the changed core 
      for(int i = 0; i < numCore; i++) {
         if( i != swapCore ) {
            if(bandwidth[changedCore][i] != 0) {
               network.removeConnection(core[changedCore].getPosition(), core[i].getPosition());
            }
            if(bandwidth[i][changedCore] != 0) {
               network.removeConnection(core[i].getPosition(), core[changedCore].getPosition());
            }
         }
      }
      //remove all connections from the old position of the swap core
      for(int i = 0; i < numCore; i++) {
         if( i != changedCore) {
            if(bandwidth[swapCore][i] != 0) {
               network.removeConnection(core[swapCore].getPosition(), core[i].getPosition());
            }
            if(bandwidth[i][swapCore] != 0) {
               network.removeConnection(core[i].getPosition(), core[swapCore].getPosition());
            }
         }
      }
      //remove connection between two cores
      if(bandwidth[changedCore][swapCore] != 0) {
         network.removeConnection(core[changedCore].getPosition(), core[swapCore].getPosition());
      }
      if(bandwidth[swapCore][changedCore] != 0) {
         network.removeConnection(core[swapCore].getPosition(), core[changedCore].getPosition());
      }
      //place core on new pos
      core[swapCore].setPosition(oldPos);
      network.addCore(oldPos, swapCore);

      core[changedCore].setPosition(newPos);
      network.addCore(newPos, changedCore);

      //add all connections of changedCore
      for(int i = 0; i < numCore; i++) {
         if(i != swapCore) {
            if(bandwidth[changedCore][i] != 0) {
               network.addConnection(core[changedCore].getPosition(), core[i].getPosition());
            }
            if(bandwidth[i][changedCore] != 0) {
               network.addConnection(core[i].getPosition(), core[changedCore].getPosition());
            }
         }
      }

      //add all connections of swap core
      for(int i = 0; i < numCore; i++) {
         if(i != changedCore) {
            if(bandwidth[swapCore][i] != 0) {
               network.addConnection(core[swapCore].getPosition(), core[i].getPosition());
            }
            if(bandwidth[i][swapCore] != 0) {
               network.addConnection(core[i].getPosition(), core[swapCore].getPosition());
            }
         }
      }
      //add connection between two cores
      if(bandwidth[changedCore][swapCore] != 0) {
         network.addConnection(core[changedCore].getPosition(), core[swapCore].getPosition());
      }
      if(bandwidth[swapCore][changedCore] != 0) {
         network.addConnection(core[swapCore].getPosition(), core[changedCore].getPosition());
      }
      cost.initCost(bandwidth, latency, core, LINK_LATENCY, network);

   } else { //new position is empty
      //remove old cost
      cost.updateCost(bandwidth, latency, LINK_LATENCY, core, changedCore, 0);

      //remove all connections from the old position
      for(int i = 0; i < numCore; i++) {
         if(bandwidth[changedCore][i] != 0) {
            network.removeConnection(core[changedCore].getPosition(), core[i].getPosition());
         }
         if(bandwidth[i][changedCore] != 0) {
            network.removeConnection(core[i].getPosition(), core[changedCore].getPosition());
         }
      }
      //move core from old pos
      network.removeCore(core[changedCore].getPosition());
      //place core on new pos
      core[changedCore].setPosition(newPos);
      network.addCore(core[changedCore].getPosition(), changedCore);
      //add all connections 
      for(int i = 0; i < numCore; i++) {
         if(bandwidth[changedCore][i] != 0) {
            network.addConnection(core[changedCore].getPosition(), core[i].getPosition());
         }
         if(bandwidth[i][changedCore] != 0) {
            network.addConnection(core[i].getPosition(), core[changedCore].getPosition());
         }
      }
      //calculate new cost
      cost.updateCost(bandwidth, latency, LINK_LATENCY, core, changedCore, 1);
      cost.calculateCost(bandwidth, core, network);
   }
   //calculate new cost
   //cost.initCost(bandwidth, latency, core, LINK_LATENCY, network);
}

void State::printState() {
   cout << "current state: " << endl;
   for(unsigned int i = 0; i < core.size(); i++) {
      core[i].printCore();
   }
   cout << "\n";
   cost.printCost();
   //network.printUtil();
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
