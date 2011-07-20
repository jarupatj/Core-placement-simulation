#pragma once

struct Coordinate {
   int x;
   int y;
};

class Core {
   public:
      Core(int x, int y);
      ~Core();

      Coordinate getPosition();
      void setPosition(int x, int y);

   private:
      Coordinate position;
};
