#pragma once

#define MAX_TURNS 8

class Router {
   public:
      Router();
      ~Router();

      int getTurns(int index); //get turns[index]
      void setTurns(int index, int num); //set turns[index] = num 
      int isPsudonode();
      bool getCore();
      void setCore(bool set);

   private:
      int turns[MAX_TURNS];
      bool hasCore;
};
