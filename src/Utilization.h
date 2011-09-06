/*
 * Utilization.h
 *
 *  Created on: Sep 6, 2011
 *      Author: nana
 */

#ifndef UTILIZATION_H_
#define UTILIZATION_H_

#define MAX_DIRECTION 4

#define NO_NODE -1

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

      Utilization& operator=(const Utilization& sourceUtil);
      void deepCopy(const Utilization& sourceUtil);

      void init(int row, int col);
      void reset();
      double calculateUtil();
      void addConnection(int nodeIdPrev, int dir, int nodeIdCur, double bw);
      double getMaxBandwidth(int& pos, int& dir) const;

   private:
      int size;
      Link **utilization;
};

#endif /* UTILIZATION_H_ */
