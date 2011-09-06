#ifndef CORE_HPP
#define CORE_HPP

#include "Defs.hpp"

class Core {
   public:
      Core(int x, int y);
      ~Core();

      /*
       * get core position
       */
      Coordinate getPosition() const;
      /*
       * set core position
       */
      void setPosition(Coordinate newPos);
      /*
       * print core position
       */
      void printCore();

   private:
      Coordinate position;
};

#endif
