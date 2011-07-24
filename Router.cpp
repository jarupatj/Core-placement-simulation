#include "Router.hpp"

Router::Router() {
   for(int i = 0; i < MAX_TURNS; i++) {
      turns[i] = 0;
   }
   hasCore = false;
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

bool Router::getCore() {
   return hasCore;
}

void Router::setCore(bool set) {
   hasCore = set;
}
