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

      /*
       * Initialize network
       */
      void init(int r, int c);
      /*
       * place a core to a network
       */
      void addCore(Coordinate pos, int coreIndex);
      /*
       * remove a core from a network
       */
      void removeCore(Coordinate pos);

      /*
       * get indexing number of a core that is placed at "pos"
       * the indexing number is used to access a core in core vector
       */
      int getCoreIndex(Coordinate pos); 
      /*
       * check if position "pos" contains a core or not
       */
      bool hasCore(Coordinate pos);
      /*
       * add/remove connection from a network
       * op specifies operation ADD/REMOVE
       */
      void changeConnection(Coordinate from, Coordinate to, int op);
      /*
       * change all connections to and from core[index]
       * op specifies operation ADD/REMOVE
       */
      void changeAllConnections(double** bandwidth, vector<Core> core, int index, int op);

      //bool isLegal(int LINK_BANDWIDTH);

      void updateNetwork(double** bandwidth, vector<Core> core);
      double calculateUtilization();

      void printNetwork() const;
      //void printUtil() const;
      /*
       * show network diagram
       * - where the cores and psudonodes are located
       */
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

#endif
