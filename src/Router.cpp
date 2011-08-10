#include <cassert>

#include "Router.hpp"

using namespace std;

Router::Router() {
   for(int i = 0; i < MAX_TURNS; i++) {
      turns[i] = 0;
   }
   coreIndex = NO_CORE;
   psudonode = false;
}

Router::~Router() {
}

int Router::getTurn(int t) {
   assert(t >= 0 && t < 8);
   return turns[t];
}

void Router::addTurn(int t) {
   assert(t >= 0 && t < 8);
   turns[t]++;
   checkPsudonode();
}

void Router::removeTurn(int t) {
   assert(t >= 0 && t < 8);
   turns[t]--;
   checkPsudonode();
}

void Router::checkPsudonode() {
   if(coreIndex != NO_CORE) {
      psudonode = true;
      return;
   }

   int t[MAX_TURNS] = {0};
   for(int i = 0; i < MAX_TURNS; i++) {
      if(turns[i] > 0) {
         t[i] = 1;
      }
   }

   //check for same input diff output
   //same input from the left
   if( (t[LEFT_RIGHT] + t[LEFT_BOTTOM] + t[LEFT_TOP]) > 1 ) {
      psudonode = true;
   } else if( (t[RIGHT_LEFT] + t[RIGHT_BOTTOM] + t[RIGHT_TOP]) > 1) {
      psudonode = true;
   } else if( (t[TOP_BOTTOM] + t[LEFT_BOTTOM] + t[RIGHT_BOTTOM]) > 1)  {
      psudonode = true;
   } else if( (t[BOTTOM_TOP] + t[LEFT_TOP] + t[RIGHT_TOP]) > 1) {
      psudonode = true;
   } else {
      psudonode = false;
   }
}

void Router::setCore(int index) {
   coreIndex = index;
   checkPsudonode();
}
