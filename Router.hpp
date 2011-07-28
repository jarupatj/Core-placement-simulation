#pragma once

#define MAX_TURNS 8
#define NO_CORE -1

#define TOP_BOTTOM   0
#define BOTTOM_TOP   1
#define LEFT_RIGHT   2
#define RIGHT_LEFT   3
#define LEFT_BOTTOM  4
#define LEFT_TOP     5
#define RIGHT_BOTTOM 6
#define RIGHT_TOP    7

class Router {
   public:
      Router();
      ~Router();

      int getTurn(int index); //get turns[index]
      void setTurn(int index, int num); //set turns[index] = num 
      void addTurn(int index);
      void removeTurn(int index);
      bool isPsudonode();
      int getCoreIndex();
      void setCore(int index);

   private:
      //variable
      int turns[MAX_TURNS];
      int coreIndex;
      bool psudonode;

      //function
      void checkPsudonode();
};
