#ifndef CORE_HPP
#define CORE_HPP

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
