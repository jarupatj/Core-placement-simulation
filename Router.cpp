#include <cassert>

#include "Router.hpp"

Router::Router() {
   for(int i = 0; i < MAX_TURNS; i++) {
      turns[i] = 0;
   }
   coreIndex = NO_CORE;
   psudonode = false;
}

Router::~Router() {
}

int Router::getTurn(int index) {
   assert(index >= 0 && index < 8);
   return turns[index];
}

void Router::setTurn(int index, int num) {
   assert(index >= 0 && index < 8);
   turns[index] = num;
}

void Router::addTurn(int index) {
   assert(index >= 0 && index < 8);
   turns[index]++;
   isPsudonode();
}

void Router::removeTurn(int index) {
   assert(index >= 0 && index < 8);
   if( turns[index] > 0 ) {
      turns[index]--;
      isPsudonode();
   }
}

void Router::checkPsudonode() {
   if(coreIndex != NO_CORE) {
      psudonode = true;
      return;
   }

   int t[MAX_TURNS] = {0};
   for(int i = 0; i < MAX_TURNS; i++) {
      if(turns[i] != 0) {
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

bool Router::isPsudonode() {
   return psudonode;
}

int Router::getCoreIndex() {
   return coreIndex;
}

void Router::setCore(int index) {
   coreIndex = index;
}
