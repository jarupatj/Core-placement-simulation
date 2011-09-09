#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

#include "Network.hpp"
#include "Core.hpp"
#include "Defs.hpp"
#include "Cost.hpp"

using std::vector;
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
   vector<Core> core;
   core.push_back(Core(0, 1));
   core.push_back(Core(1, 4));
   core.push_back(Core(4, 6));
   core.push_back(Core(3, 0));
   core.push_back(Core(4, 3));

   double** bandwidth;
   double** latency;
   bandwidth = new double*[5];
   latency = new double*[5];
   for (int i = 0; i < 5; i++) {
      bandwidth[i] = new double[5];
      memset(bandwidth[i], 0, sizeof(double) * 5);
      latency[i] = new double[5];
      memset(latency[i], 0, sizeof(double) * 5);
   }

   bandwidth[0][3] = 10;
   bandwidth[0][4] = 20;
   bandwidth[1][0] = 30;
   bandwidth[1][4] = 40;
   bandwidth[2][3] = 25;
   bandwidth[3][1] = 40;
   bandwidth[3][2] = 50;
   bandwidth[4][0] = 10;
   bandwidth[4][1] = 30;

   latency[0][3] = 10;
   latency[0][4] = 10;
   latency[1][0] = 10;
   latency[1][4] = 10;
   latency[2][3] = 10;
   latency[3][1] = 10;
   latency[3][2] = 10;
   latency[4][0] = 10;
   latency[4][1] = 10;

   Network network;
   network.init(7, 5);

   Coordinate from = { 0, 1 };
   Coordinate to = { 3, 0 };
   network.addCore(from, 0); //core1
   network.addCore(to, 3); //core4
   network.changeConnection(from, to, ADD); //1-4
   to.x = 4;
   to.y = 3;
   network.addCore(to, 4); //core5
   network.changeConnection(from, to, ADD); //1-5

   from.x = 1;
   from.y = 4;
   network.addCore(from, 1); //core2
   to.x = 0;
   to.y = 1;
   network.changeConnection(from, to, ADD); //2-1
   to.x = 4;
   to.y = 3;
   network.changeConnection(from, to, ADD); //2-5

   from.x = 4;
   from.y = 6;
   network.addCore(from, 2); //core3
   to.x = 3;
   to.y = 0;
   network.changeConnection(from, to, ADD); //3-4

   from.x = 3;
   from.y = 0;
   to.x = 1;
   to.y = 4;
   network.changeConnection(from, to, ADD); //4-2
   to.x = 4;
   to.y = 6;
   network.changeConnection(from, to, ADD); //4-3

   from.x = 4;
   from.y = 3;
   to.x = 0;
   to.y = 1;
   network.changeConnection(from, to, ADD); //5-1
   to.x = 1;
   to.y = 4;
   network.changeConnection(from, to, ADD); //5-2

   Cost cost;
   cost.init(1, 1, 0.2, 0.04);
   cost.initCost(bandwidth, latency, core, 1, network);

   network.updateUtilization(bandwidth, core);
   network.showDiagram();

   cost.printCost();
   cout << endl;

   int changedCore = 3;
   Coordinate newPos = { 4, 6 };
   int swapCore = network.getCoreIndex(newPos);
   assert(swapCore == 2);
   Coordinate oldPos = core[changedCore].getPosition();

   //remove old cost (compaction, slack, proximity)
   cost.updateCost(bandwidth, latency, 1, core, REMOVE, changedCore,
         swapCore);
   //remove all connections from the changed core
   network.changeAllConnections(bandwidth, core, changedCore, REMOVE);
   //remove all connections from the old position of the swap core
   network.changeAllConnections(bandwidth, core, swapCore, REMOVE);
   //add the overlap
   if (bandwidth[changedCore][swapCore] != 0) {
      network.changeConnection(core[changedCore].getPosition(),
            core[swapCore].getPosition(), ADD);
   }
   if (bandwidth[swapCore][changedCore] != 0) {
      network.changeConnection(core[swapCore].getPosition(),
            core[changedCore].getPosition(), ADD);
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
   if (bandwidth[changedCore][swapCore] != 0) {
      network.changeConnection(core[changedCore].getPosition(),
            core[swapCore].getPosition(), REMOVE);
   }
   if (bandwidth[swapCore][changedCore] != 0) {
      network.changeConnection(core[swapCore].getPosition(),
            core[changedCore].getPosition(), REMOVE);
   }
   //calculate new cost (compaction, slack, proximity)
   cost.updateCost(bandwidth, latency, 1, core, ADD, changedCore,
         swapCore);
   //calculate new cost
   //cost.initCost(bandwidth, latency, core, LINK_LATENCY, network);
   cost.calculateCost(bandwidth, core, network);

   network.updateUtilization(bandwidth, core);

   //network.printNetwork();

   network.showDiagram();

   cost.printCost();
   cout << endl;

   return 0;
}
