#include <iostream>
#include <cassert>
#include <cstdio>

#include "Network.hpp"

#define MAX_DIRECTION 4

Network::Network() {
   row = 0;
   col = 0;
   routers = NULL;
   utilization = NULL;
}

Network::~Network() {
   for(int i = 0; i < row; i++) {
      delete [] routers[i];
   }
   delete [] routers;
}

Network::Network(const Network& sourceNetwork) {
   deepCopy(sourceNetwork);
}

Network& Network::operator=(const Network& sourceNetwork) {
   if (this == &sourceNetwork) {
      return *this;
   }

   for(int i = 0; i < row; i++) {
      delete [] routers[i];
   }
   delete routers;

   deepCopy(sourceNetwork);

   return *this;
}

void Network::deepCopy(const Network& sourceNetwork) {
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

void Network::init(int r, int c) {
   row = r;
   col = c;

   routers = new Router* [row];
   for(int i = 0; i < row; i++) {
      routers[i] = new Router [col];
   }

   utilization = new Link* [row*col];
   for(int i = 0; i < (row*col); i++) {
      utilization[i] = new Link[MAX_DIRECTION];
   }
}

int Network::getRow() {
   return row;
}

int Network::getCol() {
   return col;
}

void Network::addCore(Coordinate pos, int coreIndex) {
   assert(pos.y < row);
   assert(pos.x < col);
   routers[pos.y][pos.x].setCore(coreIndex);
}

void Network::removeCore(Coordinate pos) {
   assert(pos.y < row);
   assert(pos.x < col);
   routers[pos.y][pos.x].setCore(NO_CORE);
}

int Network::getCoreIndex(Coordinate pos) {
   assert(pos.y < row);
   assert(pos.x < col);
   return routers[pos.y][pos.x].getCoreIndex();
}

bool Network::hasCore(Coordinate pos) {
   assert(pos.y < row);
   assert(pos.x < col);
   return routers[pos.y][pos.x].getCoreIndex() != NO_CORE;
}

void Network::addConnection(Coordinate from, Coordinate to) {
   assert(from.y < row);
   assert(from.x < col);
   assert(to.y < row);
   assert(to.x < col);

   Coordinate path;
   path.x = from.x;
   path.y = from.y;
   //move in x direction
   while( path.x != to.x ) {
      if( path.x < to.x ) { //go right
         routers[path.y][path.x].addTurn(LEFT_RIGHT);
         path.x++;
      } else { //go left
         routers[path.y][path.x].addTurn(RIGHT_LEFT);
         path.x--;
      }
   }

   //turn
   if( from.x < to.x ) { //from left
      if (from.y < to.y ) { //go up 
         routers[path.y][path.x].addTurn(LEFT_TOP);
         path.y++;
      } else if( from.y > to.y)  { //go down
         routers[path.y][path.x].addTurn(LEFT_BOTTOM);
         path.y--;
      }
   } else if( from.x > to.x ) { //from right
      if (from.y < to.y ) { //go up 
         routers[path.y][path.x].addTurn(RIGHT_TOP);
         path.y++;
      } else if( from.y > to.y)  { //go down
         routers[path.y][path.x].addTurn(RIGHT_BOTTOM);
         path.y--;
      }
   }

   //move in y direction
   while( path.y != to.y ) {
      if( path.y < to.y ) { //go up 
         routers[path.y][path.x].addTurn(BOTTOM_TOP);
         path.y++;
      } else { //go down 
         routers[path.y][path.x].addTurn(TOP_BOTTOM);
         path.y--;
      }
   }
}

void Network::removeConnection(Coordinate from, Coordinate to) {
   assert(from.y < row);
   assert(from.x < col);
   assert(to.y < row);
   assert(to.x < col);

   Coordinate path;
   path.x = from.x;
   path.y = from.y;
   //move in x direction
   while( path.x != to.x ) {
      if( path.x < to.x ) { //go right
         routers[path.y][path.x].removeTurn(LEFT_RIGHT);
         path.x++;
      } else { //go left
         routers[path.y][path.x].removeTurn(RIGHT_LEFT);
         path.x--;
      }
   }

   //turn
   if( from.x < to.x ) { //from left
      if (from.y < to.y ) { //go up 
         routers[path.y][path.x].removeTurn(LEFT_TOP);
         path.y++;
      } else if( from.y > to.y)  { //go down
         routers[path.y][path.x].removeTurn(LEFT_BOTTOM);
         path.y--;
      }
   } else if( from.x > to.x ) { //from right
      if (from.y < to.y ) { //go up 
         routers[path.y][path.x].removeTurn(RIGHT_TOP);
         path.y++;
      } else if( from.y > to.y)  { //go down
         routers[path.y][path.x].removeTurn(RIGHT_BOTTOM);
         path.y--;
      }
   }

   //move in y direction
   while( path.y != to.y ) {
      if( path.y < to.y ) { //go up 
         routers[path.y][path.x].removeTurn(BOTTOM_TOP);
         path.y++;
      } else { //go down 
         routers[path.y][path.x].removeTurn(TOP_BOTTOM);
         path.y--;
      }
   }
}

void Network::updateUtilization(int** bandwidth, int numCore, vector<Core> core) {
   int nodeIdPrev, nodeIdCur;
   Coordinate prev, cur;
   int temp;
   for(int start = 0; start < numCore; start++) {
      for(int dest = 0; dest < numCore; dest++) {
         //has a connection
         if( bandwidth[start][dest] != 0 ) {
            prev = start;
            cur = core[start].getPosition();
            while(cur.x != dest.x) {
               if( path.x < to.x ) { //go right
                  routers[path.y][path.x].removeTurn(LEFT_RIGHT);
                  path.x++;
               } else { //go left
                  routers[path.y][path.x].removeTurn(RIGHT_LEFT);
                  path.x--;
               }
               //if current router position is a psudonode
               //add connection to utilization matrix
               if( routers[cur.y][cur.x].isPsudonode() ) {
                  nodeIdPrev = prev.y * col + prev.x;
                  nodeIdCur = cur.y * col + cur.x;
                  utilization[nodeIdPrev]
               }
            }
         }
      }
   }
}

void Network::printNetwork() {
   printf("        0 1 2 3 4 5 6 7\n");
   for(int i = 0; i < row; i++) {
      for(int j = 0; j < col; j++) {
         printf("%2d,%2d = ", i, j); 
         for(int k = 0; k < 8; k++) {
            if( routers[i][j].getTurn(k) != 0 ) {
               printf("%d ", routers[i][j].getTurn(k));
            } else {
               printf("  ");
            }
         }
         printf("\n");
      }
   }

   printf("psudo nodes\n");
   for(int i = 0; i < row; i++) {
      for(int j = 0; j < col; j++) {
         if(routers[i][j].isPsudonode()) {
            printf("%d-%d\n", j, i);
         }
      }
   }
}
