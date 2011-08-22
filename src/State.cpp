#include <iostream>
#include <cmath>
#include <fstream>

#include "Defs.hpp"
#include "State.hpp"
#include "Utils.hpp"

using namespace std;

State::State() {
   bandwidth = NULL;
   latency = NULL;
   meshRow= 0;
   meshCol= 0;
}

State::~State(){
   for(unsigned int i = 0; i < core.size(); i++) {
      delete [] bandwidth[i];
      delete [] latency[i];
   }
   delete [] bandwidth;
   delete [] latency;
}

State::State(const State& sourceState) {
   deepCopy(sourceState);
}

State& State::operator=(const State& sourceState) {
   //check for self-assignment (same obj)
   if (this == &sourceState) {
      return *this;
   }

   for(unsigned int i = 0; i < core.size(); i++) {
      delete [] bandwidth[i];
      delete [] latency[i];
   }
   delete [] bandwidth;
   delete [] latency;

   core.clear();
   deepCopy(sourceState);
   return *this;
}

void State::deepCopy(const State& sourceState) {
   LINK_BANDWIDTH = sourceState.LINK_BANDWIDTH;
   LINK_LATENCY = sourceState.LINK_LATENCY;
   meshRow= sourceState.meshRow;
   meshCol= sourceState.meshCol;

   core = sourceState.core;
   network = sourceState.network;
   cost = sourceState.cost;

   if(sourceState.bandwidth) {
      //allocate memory
      bandwidth = new int* [core.size()];
      for(unsigned int i = 0; i < core.size(); i++) {
         bandwidth[i] = new int [core.size()];
      }
      //copy 
      for(unsigned int i = 0; i < core.size(); i++) {
         for(unsigned int j = 0; j < core.size(); j++) {
            bandwidth[i][j] = sourceState.bandwidth[i][j];
         }
      }
   } else {
      bandwidth = NULL;
   }
   
   if(sourceState.latency) {
      //allocate memory
      latency = new int* [core.size()];
      for(unsigned int i = 0; i < core.size(); i++) {
         latency[i] = new int [core.size()];
      }
      //copy 
      for(unsigned int i = 0; i < core.size(); i++) {
         for(unsigned int j = 0; j < core.size(); j++) {
            latency[i][j] = sourceState.latency[i][j];
         }
      }
   } else {
      latency = NULL;
   }
}

int State::init(double alpha, double beta, double gamma, double delta, \
                char* filename){
   int numCore;
   ifstream file(filename);
   if(!file.is_open()) {
      return FILE_OPEN_ERR;
   }

   file >> LINK_BANDWIDTH >> LINK_LATENCY
   >> meshRow >> meshCol >> numCore;

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

   /*
   * Initialize the network by placing cores on the network
   */
   int x,y;
   network.init(meshRow, meshCol);
   for(int i = 0; i < numCore; i++) {
      if(file.good()) {
         file >> x >> y;
      }
      core.push_back(Core(x,y));
      network.addCore(core[i].getPosition(), i);
   }

   /*
   * Read the connections from file
   * Initialize the bandwidth matrix and latency matrix
   */
   int from, to, bw, laten;
   while( file.good() ) {
      file >> from >> to >> bw >> laten;
      bandwidth[from-1][to-1] = bw;
      latency[from-1][to-1] = laten;
   }

   /*
   * Initialize the connection in the network
   */
   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         if(bandwidth[i][j] != 0) { //has a connection from i to j
            network.changeConnection(core[i].getPosition(), core[j].getPosition(), ADD);
         }
      }
   }

   if( !isLegal() ) {
      return ILLEGAL_STATE_ERR;
   }
   
   /*
   * Calculate initial cost
   */
   cost.init(alpha, beta, gamma, delta);
   cost.initCost(bandwidth, latency, core, LINK_LATENCY, network);

   file.close();

   return NO_ERR;
}

double State::getCost() {
   return cost.getCost();
}

bool State::isLegal() {
   int hops;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = 0; j < core.size(); j++) {
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

void State::generateNewState() {
   //randomly select one core
   int changedCore = uniform_n(core.size());
   //randomly select new position
   Coordinate newPos;
   newPos.x = uniform_n(meshCol);
   newPos.y = uniform_n(meshRow);
   
   //if the new position is not empty 
   if( network.hasCore(newPos) ) {
      //swap
      Coordinate oldPos = core[changedCore].getPosition();
      int swapCore = network.getCoreIndex(newPos);

      //remove all connections from the changed core 
      network.changeAllConnections(bandwidth, core, changedCore, REMOVE);
      //remove all connections from the old position of the swap core
      network.changeAllConnections(bandwidth, core, swapCore, REMOVE);
      //add the overlap
      if(bandwidth[changedCore][swapCore] != 0) {
         network.changeConnection(core[changedCore].getPosition(), core[swapCore].getPosition(), ADD);
      }
      if(bandwidth[swapCore][changedCore] != 0) {
         network.changeConnection(core[swapCore].getPosition(), core[changedCore].getPosition(), ADD);
      }

      //place core on new pos
      core[swapCore].setPosition(oldPos);
      network.addCore(oldPos, swapCore);
      core[changedCore].setPosition(newPos);
      network.addCore(newPos, changedCore);

      //add all connections of changedCore
      network.changeAllConnections(bandwidth, core, changedCore, ADD);
      //add all connections of swap core
      network.changeAllConnections(bandwidth, core, swapCore, ADD);
      //remove overlap
      if(bandwidth[changedCore][swapCore] != 0) {
         network.changeConnection(core[changedCore].getPosition(),\
                                  core[swapCore].getPosition(), REMOVE);
      }
      if(bandwidth[swapCore][changedCore] != 0) {
         network.changeConnection(core[swapCore].getPosition(), \
                                  core[changedCore].getPosition(), REMOVE);
      }
      //calculate new cost
      cost.initCost(bandwidth, latency, core, LINK_LATENCY, network);

   } else { //new position is empty
      //remove old cost
      cost.updateCost(bandwidth, latency, LINK_LATENCY, core, changedCore, REMOVE);
      //remove all connections from the old position
      network.changeAllConnections(bandwidth, core, changedCore, REMOVE);
      //move core from old pos
      network.removeCore(core[changedCore].getPosition());
      //place core on new pos
      core[changedCore].setPosition(newPos);
      network.addCore(core[changedCore].getPosition(), changedCore);
      //add all connections 
      network.changeAllConnections(bandwidth, core, changedCore, ADD);
      //calculate new cost
      cost.updateCost(bandwidth, latency, LINK_LATENCY, core, changedCore, ADD);
      cost.calculateCost(bandwidth, core, network);
   }
}

void State::printState() const {
   cost.printCost();
}

void State::printSummary() const {
   cost.printSummary();
   cout << "Maximum bandwidth in a link = " << network.getMaxBandwidthLink();
   network.showDiagram();
}
