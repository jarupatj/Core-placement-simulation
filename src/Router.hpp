#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "Defs.hpp"

class Router {
   public:
      Router();
      ~Router();

      /*
       * get turn count of turn "t"
       */
      int getTurn(int t) const;
      /*
       * add or remove turn from a router
       * op is either ADD or REMOVE
       */
      void changeTurn(int t, int op);
      /*
       * check if a router is psudonode or not
       */
      bool isPsudonode() const;
      /*
       * get indexing number of a core
       * so we can access the core in core vector
       */
      int getCoreIndex() const;
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

inline bool Router::isPsudonode() const {
   return psudonode;
}

inline int Router::getCoreIndex() const {
   return coreIndex;
}

#endif
