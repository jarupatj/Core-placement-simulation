#pragma once

#define MAX_TURNS 8
#define NO_CORE -1

enum Turn {
	TOP_BOTTOM,
	BOTTOM_TOP,
	LEFT_RIGHT,
	RIGHT_LEFT,
	LEFT_BOTTOM,
	LEFT_TOP,
	RIGHT_BOTTOM,
	RIGHT_TOP
};

class Router {
   public:
      Router();
      ~Router();

      int getTurn(int t); //get turns[t]
      void setTurn(int t, int num); //set turns[t] = num
      void addTurn(int t);
      void removeTurn(int t);
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

inline bool Router::isPsudonode() {
   return psudonode;
}

inline int Router::getCoreIndex() {
   return coreIndex;
}

inline void Router::setCore(int index) {
   coreIndex = index;
}
