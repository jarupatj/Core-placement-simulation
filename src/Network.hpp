#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>

#include "Defs.hpp"
#include "Router.hpp"
#include "Core.hpp"
#include "Utilization.hpp"

using std::vector;

class Network{
   public:
      Network();
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
      void changeAllConnections(vector< vector<double> > &bandwidth, vector<Core> &core, int index, int op);

      //bool isLegal(int LINK_BANDWIDTH);

      /*
       * Update utilization matrix by tracing route of all connections
       */
      void updateUtilization(vector< vector<double> > &bandwidth, vector<Core> &core);
      /*
       * calculate utilization using utilization matrix
       */
      double calculateUtilization();
      /*
       * printing turn counts for each router
       * and list of psudonodes
       */
      void printNetwork() const;
      /*
       * show network diagram
       * - where the cores and psudonodes are located
       */
      void showDiagram() const;
      /*
       * print the maximum bandwidth used in a link
       */
      void printMaxBandwidthLink() const;
      
   private:
      int row;
      int col;
      bool legal;
      vector< vector<Router> > routers;
      Utilization utilization;

      void deepCopy(const Network& sourceNetwork);
      /*
       * get a direction that would take you from "from" to "to"
       */
      Direction getDirection(Coordinate from, Coordinate to);
};

#endif
