#ifndef UTILIZATION_HPP_
#define UTILIZATION_HPP_

#include <vector>

#include "Defs.hpp"

#define MAX_DIRECTION 4

#define NO_NODE -1

using std::vector;

struct Link {
   int toNodeId; //link to what nodeId
   int connection; //number of connection in using this link
   double bandwidth; //bandwidth going through this link

   Link() { //constructor
      toNodeId = NO_NODE;
      connection = 0;
      bandwidth = 0;
   }
};

class Utilization {
   public:
      Utilization();
      ~Utilization();

      /*
       * initialize utilization matrix
       * allocate the memory
       */
      void init(int row, int col);
      /*
       * set all element of the matrix to zero
       */
      void reset();
      /*
       * calculate utilization cost
       */
      double calculateUtil();
      /*
       * Update utilization matrix
       * This adds connection between 2 psudonodes.
       * The connection starts from "nodeIdPrev" to "nodeIdCur"
       * in direction "dir" with bandwidth "bw"
       */
      void addConnection(int nodeIdPrev, int dir, int nodeIdCur, double bw);
      /*
       * Get the maximum bandwidth of a link in a network
       * nodeId is modified to nodeId of a utilization matrix
       * dir is modified to indicate which direction a link is located
       * relative to the node
       */
      double getMaxBandwidth(int& nodeId, int& dir) const;
      /*
       * print utilization matrix
       */
      void printUtil() const;

   private:
      int size;
      vector< vector<Link> > utilization;
};

#endif /* UTILIZATION_HPP_ */
