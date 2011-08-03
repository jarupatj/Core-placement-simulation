#include <iostream>

#include "Cost.hpp"

Cost::Cost() {
}

Cost::~Cost() {
}

void Cost::init(double alpha, double beta, double gamma, double theta) {
   this.alpha = alpha;
   this.beta = beta;
   this.gamma = gamma;
   this.theta = theta;
}

double Cost::initCost(const int** bandwidth, const int** latency, const vector<Core> core, const int LINK_LATENCY, const Network& network) {
   compaction = compactionCost(bandwidth, core);
   dilation = dilationCost(bandwidth, latency, core, LINK_LATENCY, network);
   cost = alpha * compaction + (1-alpha) * dilation;
   return cost;
}

double Cost::compactionCost(const int** bandwidth, const vector<Core> core) {
   double sum = 0;
   for(int i = 0; i < core.size(); i++) {
      for(int j = 0; j < core.size(); j++) {
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

double Cost::dilationCost(const int** bandwidth, const int** latency, const vector<Core> core, const int LINK_LATENCY, const Network& network) {
   slack = slackCost(latency, core, LINK_LATENCY);
   proximity = proximityCost(bandwidth, core);
   utilization = utilizationCost(bandwidth, core, network);
   return beta * slack + gamma * proximity + theta * utilization;
}

double Cost::slackCost(const int** latency, const vector<Core> core, const int LINK_LATENCY) {
   double sum = 0;
   int hops;
   for(int i = 0; i < core.size(); i++) {
      for(int j = 0; j < core.size(); j++) {
         if(bandwidth[i][j] != 0) {
            hops = getHops(core[i].getPosition(),core[j].getPosition());
            sum += latency[i][j] - hops * LINK_LATENCY;
         }
      }
   }
   return sum;
}

double Cost::proximityCost(const int** bandwidth, const vector<Core> core) {
   double sum = 0;
   int dist;
   for(int i = 0; i < core.size(); i++) {
      for(int j = i+1; j < core.size(); j++) {
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

double Cost::utilizationCost(const int** bandwidth, const vector<Core> core, const Network& network) {
   network.updateUtilization(bandwidth, core);
   //network.printUtil();
   return network.calculateUtilization();
}
