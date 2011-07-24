#include <iostream>
#include <cassert>

#include "Network.hpp"

Network::Network() {
   row = 0;
   col = 0;
   routers = NULL;
}

Network::~Network() {
   for(int i = 0; i < row; i++) {
      delete [] routers[i];
   }
   delete routers;
}

Network::Network(const Network& sourceNetwork) {
   row = sourceNetwork.row;
   col = sourceNetwork.col;

   if(sourceNetwork.routers) {
      routers = new Router* [row];
      for(int i = 0; i < row; i++) {
         routers[i] = new Router [col];
      }

      for(int i = 0; i < row; i++) {
         for(int j = 0; j < col; j++) {
            routers[i][j] = sourceNetwork.routers[i][j];
         }
      }
   } else {
      routers = NULL;
   }
}

Network& Network::operator=(const Network& sourceNetwork) {
   if (this == &sourceNetwork) {
      return *this;
   }

   for(int i = 0; i < row; i++) {
      delete [] routers[i];
   }
   delete routers;

   if(sourceNetwork.routers) {
      routers = new Router* [row];
      for(int i = 0; i < row; i++) {
         routers[i] = new Router [col];
      }

      for(int i = 0; i < row; i++) {
         for(int j = 0; j < col; j++) {
            routers[i][j] = sourceNetwork.routers[i][j];
         }
      }
   } else {
      routers = NULL;
   }

   return *this;
}

void Network::init(int r, int c) {
   row = r;
   col = c;

   routers = new Router* [row];
   for(int i = 0; i < row; i++) {
      routers[i] = new Router [col];
   }
}

int Network::getRow() {
   return row;
}

int Network::getCol() {
   return col;
}

void Network::addCore(Coordinate pos) {
   assert(pos.y < row);
   assert(pos.x < col);
   routers[pos.y][pos.x].setCore(true);
}

void Network::removeCore(Coordinate pos) {
   assert(pos.y < row);
   assert(pos.x < col);
   routers[pos.y][pos.x].setCore(false);
}

bool Network::hasCore(Coordinate pos) {
   assert(pos.y < row);
   assert(pos.x < col);
   return routers[pos.y][pos.x].getCore();
}
