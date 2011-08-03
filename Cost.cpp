#include <iostream>
#include <cmath>

#include "Cost.hpp"

using std::cout;
using std::endl;

Cost::Cost() {
}

Cost::~Cost() {
}

void Cost::init(double alpha, double beta, double gamma, double theta) {
   this->alpha = alpha;
   this->beta = beta;
   this->gamma = gamma;
   this->theta = theta;

}

double Cost::getCost() {
   return cost;
}

void Cost::initCost(int** bandwidth, int** latency, vector<Core> core, const int LINK_LATENCY, Network& network) {
   compaction = compactionCost(bandwidth, core);
   dilation = dilationCost(bandwidth, latency, core, LINK_LATENCY, network);
   cost = alpha * compaction + (1-alpha) * dilation;
}

double Cost::compactionCost(int** bandwidth, vector<Core> core) {
   double sum = 0;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = 0; j < core.size(); j++) {
         if(bandwidth[i][j] != 0) {
            sum += bandwidth[i][j] * getHops(core[i].getPosition(),core[j].getPosition());
         }
      }
   }
   return sum;
}

int Cost::getHops(Coordinate a, Coordinate b) {
   return fabs(a.x - b.x) + fabs(a.y - b.y);
}

double Cost::dilationCost(int** bandwidth, int** latency, vector<Core> core, const int LINK_LATENCY, Network& network) {
   slack = slackCost(latency, core, LINK_LATENCY);
   proximity = proximityCost(bandwidth, core);
   utilization = utilizationCost(bandwidth, core, network);
   return beta * slack + gamma * proximity + theta * utilization;
}

double Cost::slackCost(int** latency, vector<Core> core, const int LINK_LATENCY) {
   double sum = 0;
   int hops;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = 0; j < core.size(); j++) {
         if(latency[i][j] != 0) {
            hops = getHops(core[i].getPosition(),core[j].getPosition());
            sum += latency[i][j] - hops * LINK_LATENCY;
         }
      }
   }
   return sum;
}

double Cost::proximityCost(int** bandwidth, vector<Core> core) {
   double sum = 0;
   int dist;
   for(unsigned int i = 0; i < core.size(); i++) {
      for(unsigned int j = i+1; j < core.size(); j++) {
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

double Cost::utilizationCost(int** bandwidth, vector<Core> core, Network& network) {
   network.updateNetwork(bandwidth, core);
   //network.printUtil();
   return network.calculateUtilization();
}

void Cost::printCost() {
   cout << "cost: " << cost << endl;
   cout << "compaction: " << compaction << endl;
   cout << "dilation: " << dilation << endl;
   cout << "slack: " << slack<< endl;
   cout << "proximity: " << proximity << endl;
   cout << "utilization: " << utilization << endl;
}
