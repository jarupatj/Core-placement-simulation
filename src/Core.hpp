#ifndef CORE_H
#define CORE_H

#include "Defs.hpp"

class Core {
   public:
      Core(int x, int y);
      ~Core();

      Coordinate getPosition();
      void setPosition(Coordinate newPos);

      void printCore();

   private:
      Coordinate position;
};

#endif
