#include "Core.h"

Core::Core(int x, int y) {
   position.x = x;
   position.y = y;
}

Core::~Core() {
}

Coordinate Core::getPosition() {
   return position;
}

void Core::setPosition(int x, int y) {
   position.x = x;
   position.y = y;
}
