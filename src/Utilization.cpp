#include <iostream>
#include <iomanip>
#include <cstring>

#include "Utilization.hpp"

using namespace std;

Utilization::Utilization() {
   size = 0;
}

Utilization::~Utilization() {
}

void Utilization::init(int row, int col) {
   size = row * col;
   utilization = vector< vector<Link> > (size, vector<Link> (MAX_DIRECTION));
}

void Utilization::reset() {
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < MAX_DIRECTION; j++) {
         utilization[i][j].toNodeId = NO_NODE;
         utilization[i][j].connection = 0;
         utilization[i][j].bandwidth = 0;
      }
   }
}

void Utilization::addConnection(int nodeIdPrev, int dir, int nodeIdCur,
      double bw) {
   utilization[nodeIdPrev][dir].toNodeId = nodeIdCur;
   utilization[nodeIdPrev][dir].connection++;
   utilization[nodeIdPrev][dir].bandwidth += bw;
}

double Utilization::calculateUtil() {
   double util = 0;
   for (int i = 0; i < (size); i++) {
      for (int j = 0; j < MAX_DIRECTION; j++) {
         if (utilization[i][j].toNodeId != NO_NODE) {
            util += utilization[i][j].connection * utilization[i][j].bandwidth;
         }
      }
   }
   return util;
}

double Utilization::getMaxBandwidth(int& nodeId, int& dir) const {
   double max = 0;
   for (int i = 0; i < size; i++) {
      for (int j = 0; j < MAX_DIRECTION; j++) {
         if (utilization[i][j].toNodeId != NO_NODE) {
            if (utilization[i][j].bandwidth > max) {
               max = utilization[i][j].bandwidth;
               nodeId = i;
               dir = j;
            }
         }
      }
   }
   return max;
}

void Utilization::printUtil() const {
   cout << "util\n";
   cout << "    ";
   cout << setw(9) << "top" << setw(9) << "bottom" << setw(9) << "left"
         << setw(9) << "right\n";
   for (int i = 0; i < size; i++) {
      cout << setw(4) << i << " ";
      for (int j = 0; j < MAX_DIRECTION; j++) {
         if (utilization[i][j].toNodeId != NO_NODE) {
            cout << "<" << setw(2) << utilization[i][j].toNodeId << ","
                  << setw(2) << utilization[i][j].connection << "," << setw(2)
                  << utilization[i][j].bandwidth << ">";
         } else {
            cout << "           ";
         }
      }
      cout << endl;
   }
}

