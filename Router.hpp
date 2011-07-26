#pragma once

#define MAX_TURNS 8
#define NO_CORE -1

class Router {
   public:
      Router();
      ~Router();

      int getTurns(int index); //get turns[index]
      void setTurns(int index, int num); //set turns[index] = num 
      int isPsudonode();
      int getCoreIndex();
      void setCore(int index);

   private:
      int turns[MAX_TURNS];
      int coreIndex;
};
