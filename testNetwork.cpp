#include <iostream>
#include <vector>
#include <cstring>

#include "Network.hpp"
#include "Core.hpp"

using std::vector;

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

   network.updateUtilization(bandwidth, 5, core);

   network.printNetwork();
   std::cout << network.isLegal(50) << std::endl;

   return 0;
}
