#include <iostream>
#include <cmath>
#include <cstdio>

#include "State.hpp"

using namespace std;

State::State() {
   cost = 0;
   alpha = 1;
   beta = 0.5;
   gamma = 0.5;
   theta = 0.5;
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
   
   int from, to, bw, laten;
   while( !feof(fp) ) {
      fscanf(fp, "%d%d%d%d", &from, &to, &bw, &laten);
      bandwidth[from-1][to-1] = bw;
      latency[from-1][to-1] = laten;
   }
   fclose(fp);

   Coordinate pos;
   network.init(meshRow, meshCol);
   cout << "initial position" << endl;
   for(int i = 0; i < numCore; i++) {
      pos.x = random.uniform_n(meshCol);
      pos.y = random.uniform_n(meshRow);
      while( network.hasCore(pos) ) {
         pos.x = random.uniform_n(meshCol);
         pos.y = random.uniform_n(meshRow);
      }
      cout << "x,y: " << pos.x << " " << pos.y << endl;
      core.push_back(Core(pos.x, pos.y));
      network.addCore(pos, i);
   }

   //calculate initial cost
   calculateCost();

   return 0;
}

void State::calculateCost() {
   compaction = compactionCost();
   dilation = dilationCost();
   cost = alpha * compaction + (1-alpha) * dilation;
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
   slack = slackCost();
   proximity = proximityCost();
   utilization = utilizationCost();
   return beta * slack + gamma * proximity + theta * utilization;
}

int State::slackCost() {
   int sum = 0;
   int hops;
   for(int i = 0; i < numCore; i++) {
      for(int j = 0; j < numCore; j++) {
         if(bandwidth[i][j] != 0) {
            hops = getHops(core[i].getPosition(),core[j].getPosition());
            sum += latency[i][j] - hops * LINK_LATENCY;
         }
      }
   }
   return sum;
}

int State::proximityCost() {
   int sum = 0;
   int dist;
   for(int i = 0; i < numCore; i++) {
      for(int j = i+1; j < numCore; j++) {
         //check that there are no connections between core i and core j
         if((bandwidth[i][j] == 0) && (bandwidth[j][i] == 0)) {
            Coordinate posi = core[i].getPosition();
            Coordinate posj = core[j].getPosition();
            dist = fabs(posi.x - posj.x) + fabs(posi.y - posj.y); 
            sum -= dist;
         }
      }
   }
   return sum;
}

int State::utilizationCost() {
   network.updateUtilization();
   return network.calculateUtilization();
}

bool State::isLegal() {
   int hops;
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
   
   //cout << "c core: " << changedCore << " newPos: " << newPos.x << "," << newPos.y << endl;

   //check if that pos is empty or not
   if( network.hasCore(newPos) ) {
      //remove old cost
      //swap
      Coordinate oldPos = core[changedCore].getPosition();
      int swapIndex = network.getCoreIndex(newPos);
      //place core on new pos
      core[swapIndex].setPosition(oldPos);
      network.addCore(oldPos, swapIndex);

      core[changedCore].setPosition(newPos);
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
   cout << "compaction: " << compaction << endl;
   cout << "dilation: " << dilation << endl;
   cout << "slack: " << slack<< endl;
   cout << "proximity: " << proximity << endl;
   cout << "utilization: " << utilization << endl;
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
