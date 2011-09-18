#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

#include "Defs.hpp"
#include "Cost.hpp"
#include "Utils.hpp"

using namespace std;

Cost::Cost() {
}

Cost::~Cost() {
}

void Cost::init(double alpha, double beta, double gamma, double delta) {
   this->alpha = alpha;
   this->beta = beta;
   this->gamma = gamma;
   this->delta = delta;

}

double Cost::getCost() const {
   return cost;
}

void Cost::initCost(double bandwidth[][MAX_CORE_SIZE], double latency[][MAX_CORE_SIZE], vector<Core> core,
      const double LINK_LATENCY, Network& network) {
   compaction = initCompaction(bandwidth, core);
   dilation = initDilation(bandwidth, latency, core, LINK_LATENCY, network);
   cost = alpha * compaction + (1 - alpha) * dilation;
}

double Cost::initCompaction(double bandwidth[][MAX_CORE_SIZE], vector<Core> core) {
   double sum = 0;
   for (unsigned int i = 0; i < core.size(); i++) {
      for (unsigned int j = 0; j < core.size(); j++) {
         if (bandwidth[i][j] != 0) {
            sum += bandwidth[i][j] * getHops(core[i].getPosition(),
                  core[j].getPosition());
         }
      }
   }
   return sum;
}

double Cost::initDilation(double bandwidth[][MAX_CORE_SIZE], double latency[][MAX_CORE_SIZE],
      vector<Core> core, const double LINK_LATENCY, Network& network) {
   slack = initSlack(latency, core, LINK_LATENCY);
   proximity = initProximity(bandwidth, core);
   utilization = utilizationCost(bandwidth, core, network);
   return beta * slack + gamma * proximity + delta * utilization;
}

double Cost::initSlack(double latency[][MAX_CORE_SIZE], vector<Core> core,
      const double LINK_LATENCY) {
   double sum = 0;
   int hops;
   for (unsigned int i = 0; i < core.size(); i++) {
      for (unsigned int j = 0; j < core.size(); j++) {
         if (latency[i][j] != 0) {
            hops = getHops(core[i].getPosition(), core[j].getPosition());
            sum += latency[i][j] - hops * LINK_LATENCY;
         }
      }
   }
   return sum;
}

double Cost::initProximity(double bandwidth[][MAX_CORE_SIZE], vector<Core> core) {
   double sum = 0;
   int dist;
   for (unsigned int i = 0; i < core.size(); i++) {
      for (unsigned int j = i + 1; j < core.size(); j++) {
         //check that there are no connections between core i and core j
         if ((bandwidth[i][j] == 0) && (bandwidth[j][i] == 0)) {
            dist = getHops(core[i].getPosition(), core[j].getPosition());
            sum -= dist;
         }
      }
   }
   return sum;
}

double Cost::utilizationCost(double bandwidth[][MAX_CORE_SIZE], vector<Core> core,
      Network& network) {
   network.updateUtilization(bandwidth, core);
   return network.calculateUtilization();
}

void Cost::calculateCost(double bandwidth[][MAX_CORE_SIZE], vector<Core> core,
      Network& network) {
   utilization = utilizationCost(bandwidth, core, network);
   dilation = beta * slack + gamma * proximity + delta * utilization;
   cost = alpha * compaction + (1 - alpha) * dilation;
}

void Cost::updateCost(double bandwidth[][MAX_CORE_SIZE], double latency[][MAX_CORE_SIZE],
      double LINK_LATENCY, vector<Core> core, int op, int coreA, int coreB) {
   if (op == REMOVE) {
      compaction -= changeCompaction(bandwidth, core, coreA, coreB);
      slack -= changeSlack(latency, LINK_LATENCY, core, coreA, coreB);
      proximity -= changeProximity(bandwidth, core, coreA, coreB);
   } else {
      compaction += changeCompaction(bandwidth, core, coreA, coreB);
      slack += changeSlack(latency, LINK_LATENCY, core, coreA, coreB);
      proximity += changeProximity(bandwidth, core, coreA, coreB);
   }
}

double Cost::changeCompaction(double bandwidth[][MAX_CORE_SIZE], vector<Core> core, int coreA,
      int coreB) {
   double change = 0;
   for (unsigned int i = 0; i < core.size(); i++) {
      /*
       * calculate compaction cost of connection from/to coreA
       * but do not calculate cost of connection between coreA and coreB
       * to prevent calculating duplicates
       */
      if ((int) i != coreB) {
         //connection from coreA to node i
         change += bandwidth[coreA][i] * getHops(core[coreA].getPosition(),
               core[i].getPosition());
         //connection from node i to coreA
         change += bandwidth[i][coreA] * getHops(core[i].getPosition(),
               core[coreA].getPosition());
      }
      /*
       * calculate compaction cost of connection from/to coreB
       */
      if (coreB != NO_CORE && (int)i != coreA) {
         //connection from coreB to node i
         change += bandwidth[coreB][i] * getHops(core[coreB].getPosition(),
               core[i].getPosition());
         //connection from node i to coreB
         change += bandwidth[i][coreB] * getHops(core[i].getPosition(),
               core[coreB].getPosition());
      }
   }
   /*
    * calculate cost of connection between coreA and coreB
    */
   if (coreB != NO_CORE) {
      //connection from coreB to node i
      change += bandwidth[coreA][coreB] * getHops(core[coreA].getPosition(),
            core[coreB].getPosition());
      //connection from node i to coreB
      change += bandwidth[coreB][coreA] * getHops(core[coreB].getPosition(),
            core[coreB].getPosition());
   }
   return change;
}

double Cost::changeSlack(double latency[][MAX_CORE_SIZE], const double LINK_LATENCY, vector<
      Core> core, int coreA, int coreB) {
   double change = 0;
   int hops;
   for (unsigned int i = 0; i < core.size(); i++) {
      /*
       * calculate slack cost of connection from/to coreA
       * but do not calculate cost of connection between coreA and coreB
       * to prevent calculating duplicates
       */
      if ((int) i != coreB) {
         //connection from index to node i
         if (latency[coreA][i] != 0) {
            hops = getHops(core[coreA].getPosition(), core[i].getPosition());
            change += latency[coreA][i] - hops * LINK_LATENCY;
         }
         //connection from node i to index
         if (latency[i][coreA] != 0) {
            hops = getHops(core[i].getPosition(), core[coreA].getPosition());
            change += latency[i][coreA] - hops * LINK_LATENCY;
         }
      }
      /*
       * calculate slack cost of connection from/to coreB
       */
      if (coreB != NO_CORE && (int) i != coreA) {
         //connection from index to node i
         if (latency[coreB][i] != 0) {
            hops = getHops(core[coreB].getPosition(), core[i].getPosition());
            change += latency[coreB][i] - hops * LINK_LATENCY;
         }
         //connection from node i to index
         if (latency[i][coreB] != 0) {
            hops = getHops(core[i].getPosition(), core[coreB].getPosition());
            change += latency[i][coreB] - hops * LINK_LATENCY;
         }
      }
   }
   /*
    * calculate slack cost of connections between coreA and coreB
    */
   if (coreB != NO_CORE) {
      if (latency[coreA][coreB] != 0) {
         hops = getHops(core[coreA].getPosition(), core[coreB].getPosition());
         change += latency[coreA][coreB] - hops * LINK_LATENCY;
      }
      if (latency[coreB][coreA] != 0) {
         hops = getHops(core[coreB].getPosition(), core[coreA].getPosition());
         change += latency[coreB][coreA] - hops * LINK_LATENCY;
      }
   }

   return change;
}

double Cost::changeProximity(double bandwidth[][MAX_CORE_SIZE], vector<Core> core, int coreA,
      int coreB) {
   double change = 0;
   int dist;
   for (unsigned int i = 0; i < core.size(); i++) {
      /*
       * calculate proximity cost of connection from/to core A
       * do not calculate proximity cost between coreA and coreB
       * to prevent calculating duplicates
       */
      if ((int) i != coreB) {
         if (bandwidth[coreA][i] == 0 && bandwidth[i][coreA] == 0) {
            dist = getHops(core[i].getPosition(), core[coreA].getPosition());
            change -= dist;
         }
      }
      /*
       * calculate proximity cost of connection from/to core B
       */
      if (coreB != NO_CORE && (int) i != coreA) {
         if (bandwidth[coreB][i] == 0 && bandwidth[i][coreB] == 0) {
            dist = getHops(core[i].getPosition(), core[coreB].getPosition());
            change -= dist;
         }
      }
   }
   /*
    * calculate proximity cost of connection between coreA and core B
    */
   if (coreB != NO_CORE) {
      if (bandwidth[coreA][coreB] == 0 && bandwidth[coreB][coreA] == 0) {
         dist = getHops(core[coreB].getPosition(), core[coreA].getPosition());
         change -= dist;
      }
   }
   return change;
}

void Cost::printCost() const {
   cout << right << setiosflags(ios::fixed) << setprecision(3) << setw(12)
         << cost << setw(12) << compaction << setw(12) << dilation << setw(12)
         << slack << setw(12) << proximity << setw(12) << utilization;
}

string Cost::printQuiet() const {
   stringstream str;
   /*
   str << right << setiosflags(ios::fixed) << setprecision(3) << cost << " "
         << compaction << " " << dilation << " " << slack << " " << proximity
         << " " << utilization;
   */
   str << right << setiosflags(ios::fixed) << setprecision(2) << setw(12) << cost << setw(12)
            << compaction << setw(12) << dilation << setw(12) << slack << setw(12) << proximity
            << setw(12) << utilization;
   return str.str();
}

void Cost::printSummary() const {
   cout << setiosflags(ios::fixed) << setprecision(3) << "# " << "Alpha: "
         << alpha << "\tBeta: " << beta << "\tGamma: " << gamma << "\tDelta: "
         << delta << "\n# Cost: " << cost << "\tCompaction: " << compaction
         << "\tDilation: " << dilation << "\n# Slack: " << slack
         << "\tProximity: " << proximity << "\tUtilization: " << utilization
         << endl;
}
