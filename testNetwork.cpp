#include <iostream>
#include <vector>
#include <cstring>
#include <cassert>

#include "Network.hpp"
#include "Core.hpp"

using std::vector;
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
   vector<Core> core;
   core.push_back(Core(0,1));
   core.push_back(Core(1,4));
   core.push_back(Core(4,6));
   core.push_back(Core(3,0));
   core.push_back(Core(4,3));

   int** bandwidth;
   bandwidth = new int* [5];
   for(int i = 0; i < 5; i++) {
      bandwidth[i] = new int [5];
      memset(bandwidth[i], 0, sizeof(int));
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

   Network network;
   network.init(7,5);

   Coordinate from = {0,1};
   Coordinate to = {3,0};
   network.addCore(from, 0); //core1
   network.addCore(to, 3); //core4
   network.addConnection(from, to); //1-4
   to.x = 4;
   to.y = 3;
   network.addCore(to, 4); //core5
   network.addConnection(from, to); //1-5

   from.x = 1;
   from.y = 4;
   network.addCore(from, 1); //core2
   to.x = 0;
   to.y = 1;
   network.addConnection(from, to); //2-1
   to.x = 4;
   to.y = 3;
   network.addConnection(from, to); //2-5

   from.x = 4;
   from.y = 6;
   network.addCore(from, 2); //core3
   to.x = 3;
   to.y = 0;
   network.addConnection(from, to); //3-4

   from.x = 3;
   from.y = 0;
   to.x = 1;
   to.y = 4;
   network.addConnection(from, to); //4-2
   to.x = 4;
   to.y = 6;
   network.addConnection(from, to); //4-3

   from.x = 4;
   from.y = 3;
   to.x = 0;
   to.y = 1;
   network.addConnection(from, to); //5-1
   to.x = 1;
   to.y = 4;
   network.addConnection(from, to); //5-2

   network.updateNetwork(bandwidth, core);

   int changedCore = 3;
   Coordinate newPos = {4,6};
   int swapCore = network.getCoreIndex(newPos);
   assert(swapCore == 2);
   Coordinate oldPos = core[changedCore].getPosition();
   /*

      for(unsigned int i = 0; i < core.size(); i++) {
         if( i != (unsigned int)swapCore ) {
            if(bandwidth[changedCore][i] != 0) {
               network.removeConnection(core[changedCore].getPosition(), core[i].getPosition());
            }
            if(bandwidth[i][changedCore] != 0) {
               network.removeConnection(core[i].getPosition(), core[changedCore].getPosition());
            }
         }
      }
      //remove all connections from the old position of the swap core
      for(unsigned int i = 0; i < core.size(); i++) {
         if( i != (unsigned int)changedCore) {
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
      for(unsigned int i = 0; i < core.size(); i++) {
         if(i != (unsigned int)swapCore) {
            if(bandwidth[changedCore][i] != 0) {
               network.addConnection(core[changedCore].getPosition(), core[i].getPosition());
            }
            if(bandwidth[i][changedCore] != 0) {
               network.addConnection(core[i].getPosition(), core[changedCore].getPosition());
            }
         }
      }
      //add all connections of swap core
      for(unsigned int i = 0; i < core.size(); i++) {
         if(i != (unsigned int)changedCore) {
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

      */
   /*
   //remove all connections from the changed core 
   network.removeAllConnections(bandwidth, core, changedCore);
      //remove all connections from the old position of the swap core
      network.removeAllConnections(bandwidth, core, swapCore);
      //add the overlap
      if(bandwidth[changedCore][swapCore] != 0) {
         network.addConnection(core[changedCore].getPosition(), core[swapCore].getPosition());
      }
      if(bandwidth[swapCore][changedCore] != 0) {
         network.addConnection(core[swapCore].getPosition(), core[changedCore].getPosition());
      }

      //place core on new pos
      core[swapCore].setPosition(oldPos);
      network.addCore(oldPos, swapCore);

      core[changedCore].setPosition(newPos);
      network.addCore(newPos, changedCore);

      //add all connections of changedCore
      network.addAllConnections(bandwidth, core, changedCore);

      //add all connections of swap core
      network.addAllConnections(bandwidth, core, swapCore);
      //remove overlap
      if(bandwidth[changedCore][swapCore] != 0) {
         network.removeConnection(core[changedCore].getPosition(), core[swapCore].getPosition());
      }
      if(bandwidth[swapCore][changedCore] != 0) {
         network.removeConnection(core[swapCore].getPosition(), core[changedCore].getPosition());
      }

      */
   /*
   network.removeAllConnections(bandwidth, core, changedCore);
   //move core from old pos
   network.removeCore(core[changedCore].getPosition());
   //place core on new pos
   core[changedCore].setPosition(newPos);
   network.addCore(core[changedCore].getPosition(), changedCore);
   //add all connections 
   network.addAllConnections(bandwidth, core, changedCore);
   */

   network.updateNetwork(bandwidth, core);

   network.printNetwork();

   network.showDiagram();

   return 0;
}
