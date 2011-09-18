#include <cassert>

#include "Router.hpp"

using namespace std;

Router::Router() {
   for (int i = 0; i < MAX_TURNS; i++) {
      turns[i] = 0;
   }
   coreIndex = NO_CORE;
   psudonode = false;
}

Router::~Router() {
}

int Router::getTurn(int t) const{
   assert(t >= 0 && t < 8);
   return turns[t];
}

void Router::changeTurn(int t, int op) {
   assert(t >= 0 && t < 8);
   /*
    * op == add then we add turn count "t"
    * op == remove then we remove turn count "t"
    */
   (op == ADD) ? (turns[t]++) : (turns[t]--);
   /*
    * re-checking psudonode condition every time
    * we change turn counts
    */
   checkPsudonode();
}

void Router::checkPsudonode() {
   /*
    * if router contains a core then it
    * automatically is a psudonode
    */
   if (coreIndex != NO_CORE) {
      psudonode = true;
      return;
   }
   /*
    * t corresponds to turn counts
    * t[i] = 1 if the turn count[i] is not zero
    */
   int t[MAX_TURNS] = { 0 };
   for (int i = 0; i < MAX_TURNS; i++) {
      if (turns[i] > 0) {
         t[i] = 1;
      }
   }

   /*
    * Check for psudonode condition
    * - same input different output
    * - diff input same output
    * t[i] tells us whether a router has that turn or not
    * so grouping and summing t[i] will tell us if
    * the node is a psudonode or not
    */
   if ((t[LEFT_RIGHT] + t[LEFT_BOTTOM] + t[LEFT_TOP]) > 1) {
      psudonode = true;
   } else if ((t[RIGHT_LEFT] + t[RIGHT_BOTTOM] + t[RIGHT_TOP]) > 1) {
      psudonode = true;
   } else if ((t[TOP_BOTTOM] + t[LEFT_BOTTOM] + t[RIGHT_BOTTOM]) > 1) {
      psudonode = true;
   } else if ((t[BOTTOM_TOP] + t[LEFT_TOP] + t[RIGHT_TOP]) > 1) {
      psudonode = true;
   } else {
      psudonode = false;
   }
}

void Router::setCore(int index) {
   coreIndex = index;
   /*
    * updating psudonode condition
    * when the core is added/removed
    */
   checkPsudonode();
}
