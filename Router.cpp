#include "Router.hpp"

Router::Router() {
   for(int i = 0; i < MAX_TURNS; i++) {
      turns[i] = 0;
   }
   coreIndex = NO_CORE;
}

Router::~Router() {
}

int Router::getTurns(int index) {
   return turns[index];
}

void Router::setTurns(int index, int num) {
   turns[index] = num;
}

int Router::isPsudonode() {
   return 0;
}

int Router::getCoreIndex() {
   return coreIndex;
}

void Router::setCore(int index) {
   coreIndex = index;
}
