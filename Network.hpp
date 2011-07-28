#pragma once

#include "Router.hpp"
#include "Core.hpp"

using std::vector;

#define TOP    0
#define BOTTOM 1
#define LEFT   2
#define RIGHT  3

struct Link {
   int toNodeId; //link to what nodeId
   int connection; //number of connection in using this link
   int bandwidth; //bandwidth going through this link
};

class Network{
   public:
      Network();
      Network(const Network& sourceNetwork);
      Network& operator=(const Network& sourceNetwork); 
      ~Network();

      void init(int r, int c);
      int getRow();
      int getCol();
      void addCore(Coordinate pos, int coreIndex);
      void removeCore(Coordinate pos);
      int getCoreIndex(Coordinate pos); 
      bool hasCore(Coordinate pos);//check if position has a core or not
      void addConnection(Coordinate from, Coordinate to);
      void removeConnection(Coordinate from, Coordinate to);

      void updateUtilization(int** bandwidth, int numCore, vector<Core> core);

      void printNetwork();
      
   private:
      int row;
      int col;
      Router **routers;
      Link **utilization;

      void deepCopy(const Network& sourceNetwork);
};
