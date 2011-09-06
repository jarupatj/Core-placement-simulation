/*
 * Utilization.cpp
 *
 *  Created on: Sep 6, 2011
 *      Author: nana
 */

#include <iostream>

#include "Utilization.h"

Utilization::Utilization() {
   utilization = NULL;
   size = 0;
}

Utilization::~Utilization() {
   if (utilization != NULL) {
      for (int i = 0; i < (size); i++) {
         delete[] utilization[i];
      }
      delete[] utilization;
   }
}

Utilization& Utilization::operator=(const Utilization& sourceUtil) {
   if (this == &sourceUtil) {
      return *this;
   }

   if (utilization != NULL) {
      for (int i = 0; i < (size); i++) {
         delete[] utilization[i];
      }
      delete[] utilization;
   }

   deepCopy(sourceUtil);

   return *this;
}

void Utilization::deepCopy(const Utilization& sourceUtil) {
   size = sourceUtil.size;

   if (sourceUtil.utilization) {
      utilization = new Link*[size];
      for (int i = 0; i < (size); i++) {
         utilization[i] = new Link[MAX_DIRECTION];
      }

      for (int i = 0; i < (size); i++) {
         for (int j = 0; j < MAX_DIRECTION; j++) {
            utilization[i][j] = sourceUtil.utilization[i][j];
         }
      }
   } else {
      utilization = NULL;
   }
}

void Utilization::init(int row, int col) {
   size = row * col;

   if (utilization != NULL) {
      for (int i = 0; i < (size); i++) {
         delete[] utilization[i];
      }
      delete[] utilization;
   }

   utilization = new Link*[size];
   for (int i = 0; i < (size); i++) {
      utilization[i] = new Link[MAX_DIRECTION];
   }
}

void Utilization::reset() {
   for(int i = 0; i < size; i++) {
      for(int j = 0; j < MAX_DIRECTION; j++) {
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

double Utilization::getMaxBandwidth(int& pos, int& dir) const {
   double max = 0;
   for (int i = 0; i < (size); i++) {
      for (int j = 0; j < MAX_DIRECTION; j++) {
         if (utilization[i][j].toNodeId != NO_NODE) {
            if (utilization[i][j].bandwidth > max) {
               max = utilization[i][j].bandwidth;
               pos = i;
               dir = j;
            }
         }
      }
   }
   return max;
}

