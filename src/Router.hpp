#ifndef ROUTER_HPP
#define ROUTER_HPP

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

      /*
       * get turn count of turn "t"
       */
      int getTurn(int t);
      /*
       * add or remove turn from a router
       * op is either ADD or REMOVE
       */
      void changeTurn(int t, int op);
      /*
       * check if a router is psudonode or not
       */
      bool isPsudonode();
      /*
       * get indexing number of a core
       * so we can access the core in core vector
       */
      int getCoreIndex();
      /*
       * place a core to a router
       * index corresponds to vector indexing
       * of core vector
       */
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

#endif
