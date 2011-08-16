#include <iostream>

#include "Core.hpp"

Core::Core(int x, int y) {
   position.x = x;
   position.y = y;
}

Core::~Core() {
}

Coordinate Core::getPosition() const{
   return position;
}

void Core::setPosition(Coordinate newPos) {
   position.x = newPos.x;
   position.y = newPos.y;
}

void Core::printCore() {
   std::cout << "(" << position.x << "," << position.y << ") ";
}
