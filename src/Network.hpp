#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

#include "Defs.hpp"
#include "Router.hpp"
#include "Core.hpp"

using std::vector;

enum Direction {
	NO_DIR = -1,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

#define MAX_DIRECTION 4

#define NO_NODE -1

struct Link {
   int toNodeId; //link to what nodeId
   int connection; //number of connection in using this link
   int bandwidth; //bandwidth going through this link

   Link() { //constructor
      toNodeId = NO_NODE;
      connection = 0;
      bandwidth = 0;
   }
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
      void changeConnection(Coordinate from, Coordinate to, int op);
      void changeAllConnections(int** bandwidth, vector<Core> core, int index, int op);

      bool isLegal(int LINK_BANDWIDTH);

      void updateNetwork(int** bandwidth, vector<Core> core);
      double calculateUtilization();

      void printNetwork() const;
      void printUtil() const;
      void showDiagram() const;
      void printMaxBandwidthLink() const;
      
   private:
      int row;
      int col;
      bool legal;
      Router **routers;
      Link **utilization;

      void deepCopy(const Network& sourceNetwork);
      Direction getDirection(Coordinate from, Coordinate to);
};

inline int Network::getRow() {
   return row;
}

inline int Network::getCol() {
   return col;
}

#endif
