#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

#include "Defs.hpp"
#include "Router.hpp"
#include "Core.hpp"
#include "Utilization.h"

using std::vector;

enum Direction {
	NO_DIR = -1,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
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
      void changeAllConnections(double** bandwidth, vector<Core> core, int index, int op);

      //bool isLegal(int LINK_BANDWIDTH);

      void updateNetwork(double** bandwidth, vector<Core> core);
      double calculateUtilization();

      void printNetwork() const;
      //void printUtil() const;
      void showDiagram() const;
      void printMaxBandwidthLink() const;
      
   private:
      int row;
      int col;
      bool legal;
      Router **routers;
      Utilization utilization;

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
