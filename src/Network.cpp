#include <iostream>
#include <iomanip>
#include <cassert>
#include <cstdio>

#include "Network.hpp"

using namespace std;

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

   if( utilization != NULL ) {
      for(int i = 0; i < (row*col); i++) {
         delete [] utilization[i];
      }
      delete [] utilization;
   }
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
   delete [] routers;

   if( utilization != NULL ) {
      for(int i = 0; i < (row*col); i++) {
         delete [] utilization[i];
      }
      delete [] utilization;
   }

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

   if(sourceNetwork.utilization) {
      utilization = new Link* [row*col];
      for(int i = 0; i < (row*col); i++) {
         utilization[i] = new Link[MAX_DIRECTION];
      }

      for(int i = 0; i < (row*col); i++) {
         for(int j = 0; j < MAX_DIRECTION; j++) {
            utilization[i][j] = sourceNetwork.utilization[i][j];
         }
      }
   } else {
      utilization = NULL;
   }
}

void Network::init(int r, int c) {
   row = r;
   col = c;

   routers = new Router* [row];
   for(int i = 0; i < row; i++) {
      routers[i] = new Router [col];
   }
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
   /*
   * Move in x direction
   */
   while( path.x != to.x ) {
      if( path.x < to.x ) { //go right
         routers[path.y][path.x].addTurn(LEFT_RIGHT);
         path.x++;
      } else { //go left
         routers[path.y][path.x].addTurn(RIGHT_LEFT);
         path.x--;
      }
   }

   /*
   * Turn
   */
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

   /*
   * Move in y direction
   */
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
   /*
   * Move in x direction
   */
   while( path.x != to.x ) {
      if( path.x < to.x ) { //go right
         routers[path.y][path.x].removeTurn(LEFT_RIGHT);
         path.x++;
      } else { //go left
         routers[path.y][path.x].removeTurn(RIGHT_LEFT);
         path.x--;
      }
   }

   /*
   * Turn
   */
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

   /*
   * Move in y direction
   */
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

void Network::removeAllConnections(int** bandwidth, vector<Core> core, int index) {
   for(unsigned int i = 0; i < core.size(); i++) {
      if(bandwidth[index][i] != 0) {
         removeConnection(core[index].getPosition(), core[i].getPosition());
      }
      if(bandwidth[i][index] != 0) {
         removeConnection(core[i].getPosition(), core[index].getPosition());
      }
   }
}

void Network::addAllConnections(int** bandwidth, vector<Core> core, int index) {
   for(unsigned int i = 0; i < core.size(); i++) {
      if(bandwidth[index][i] != 0) {
         addConnection(core[index].getPosition(), core[i].getPosition());
      }
      if(bandwidth[i][index] != 0) {
         addConnection(core[i].getPosition(), core[index].getPosition());
      }
   }
}

void Network::updateNetwork(int** bandwidth, vector<Core> core) {
   int nodeIdPrev, nodeIdCur;
   Direction dir;
   Coordinate prev, cur, dNode;

   if( utilization != NULL ) {
      for(int i = 0; i < (row*col); i++) {
         delete [] utilization[i];
      }
      delete [] utilization;
   }

   utilization = new Link* [row*col];
   for(int i = 0; i < (row*col); i++) {
      utilization[i] = new Link[MAX_DIRECTION];
   }

   for(unsigned int start = 0; start < core.size(); start++) {
      for(unsigned int dest = 0; dest < core.size(); dest++) {
         //has a connection
         if( bandwidth[start][dest] != 0 ) {
            prev = core[start].getPosition();
            cur = core[start].getPosition();
            dNode = core[dest].getPosition();
            //move in x
            while(cur.x != dNode.x) {
               if( cur.x < dNode.x ) { //go right
                  cur.x++;
               } else { //go left
                  cur.x--;
               }
               //if current router position is a psudonode
               //add connection to utilization matrix
               if( routers[cur.y][cur.x].isPsudonode() ) {
                  nodeIdPrev = prev.y * col + prev.x;
                  nodeIdCur = cur.y * col + cur.x;
                  dir = getDirection(prev, cur);
                  if( dir != NO_DIR ) {
                     utilization[nodeIdPrev][dir].toNodeId = nodeIdCur;
                     utilization[nodeIdPrev][dir].connection++;
                     utilization[nodeIdPrev][dir].bandwidth += bandwidth[start][dest];
                     prev = cur;
                  }
               }
            }
            //move in y 
            while( cur.y != dNode.y ) {
               if( cur.y < dNode.y ) { //go up 
                  cur.y++;
               } else { //go down 
                  cur.y--;
               }
               //if current router position is a psudonode
               //add connection to utilization matrix
               if( routers[cur.y][cur.x].isPsudonode() ) {
                  nodeIdPrev = prev.y * col + prev.x;
                  nodeIdCur = cur.y * col + cur.x;
                  dir = getDirection(prev, cur);
                  if( dir != NO_DIR ) {
                     utilization[nodeIdPrev][dir].toNodeId = nodeIdCur;
                     utilization[nodeIdPrev][dir].connection++;
                     utilization[nodeIdPrev][dir].bandwidth += bandwidth[start][dest];
                     prev = cur;
                  }
               }
            }
         }
      }
   }
}

double Network::calculateUtilization() {
   double util = 0;
   for(int i = 0; i < (row*col); i++) {
      for(int j = 0; j < MAX_DIRECTION; j++) {
         if( utilization[i][j].toNodeId != NO_NODE ) {
            util += utilization[i][j].connection * utilization[i][j].bandwidth;
         }
      }
   }
   return util;
}

Direction Network::getDirection(Coordinate from, Coordinate to) {
   if( from.x < to.x ) {
      return RIGHT;
   } else if( from.x > to.x ) {
      return LEFT;
   } else {
      if( from.y < to.y ) {
         return TOP;
      } else if( from.y > to.y ) {
         return BOTTOM;
      } else {
         return NO_DIR;
      }
   }
}

bool Network::isLegal(int LINK_BANDWIDTH) {
   int legal = true;
   for(int i = 0; i < (row*col); i++) {
      for(int j = 0; j < MAX_DIRECTION; j++) {
         if( utilization[i][j].toNodeId != NO_NODE ) {
            if( utilization[i][j].bandwidth > LINK_BANDWIDTH ) {
               legal = false;
            }
         }
      }
   }
   return legal;
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

   printUtil();
}

void Network::showDiagram() const {
   int index;
   for(int r = row-1; r >= 0; r--) {
      cout << "    ";
      //draw top link only
      for(int c = 0; c < col; c++){
         //check that current router has outgoing link to the top router
         if(routers[r][c].getTurn(BOTTOM_TOP) > 0 || routers[r][c].getTurn(LEFT_TOP) > 0
               || routers[r][c].getTurn(RIGHT_TOP) > 0) {
            cout << "  |  ";
         } else if( (r < row-1 )&&
                    (routers[r+1][c].getTurn(TOP_BOTTOM) > 0 ||
                     routers[r+1][c].getTurn(LEFT_BOTTOM) > 0||
                     routers[r+1][c].getTurn(RIGHT_BOTTOM) > 0)) {
         //check that the top router has outgoing link to the bottom router (current router)
            cout << "  |  ";
         } else {
            cout << "     ";
         }
      }
      cout << endl;
      //print row number
      cout << setw(4) << r << "  ";
      //draw right side link only
      for(int c = 0; c < col; c++){
         if(routers[r][c].isPsudonode()) {
            index = routers[r][c].getCoreIndex();
            if(index != NO_CORE) {
               cout << index+1;
            } else {
               cout << "@";
            }
         } else { //not a node
            cout << ".";
         }
         //check that the current router position has output to right side (L->R)
         //or input from the right side (R->B), (R->T)
         if(routers[r][c].getTurn(LEFT_RIGHT) > 0 || routers[r][c].getTurn(RIGHT_TOP) > 0
               || routers[r][c].getTurn(RIGHT_BOTTOM) > 0) {
            cout << "----";
         } else if ( (c < col-1) && (routers[r][c+1].getTurn(RIGHT_LEFT) > 0) ) {
         //check that the next right router has outgoint connection from to this router
            cout << "----";
         } else {
            cout << "    ";
         }
      }
      cout << endl;
   }
   cout << endl;
   //print column number
   cout << "      ";
   for(int i = 0; i < col; i++) {
      cout << i << "    ";
   }
   cout << endl;
}


void Network::printUtil() {
   printf("util\n");
   printf("    %9s%9s%9s%9s\n", "top", "bottom", "left", "right");
   for(int i = 0; i < (row*col); i++) {
      printf("%4d ", i);
      for(int j = 0; j < MAX_DIRECTION; j++) {
         if(utilization[i][j].toNodeId != NO_NODE) {
            printf("<%2d,%2d,%2d> ", utilization[i][j].toNodeId, utilization[i][j].connection, utilization[i][j].bandwidth);
         } else {
            printf("           ");
         }
      }
      printf("\n");
   }

   printf("util = %f\n", calculateUtilization());
}
