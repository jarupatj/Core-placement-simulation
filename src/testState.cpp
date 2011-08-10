#include <iostream>

#include "State.hpp"

using namespace std;

int main(int argc, char *argv[]) {
   State b;

   cout << &b << endl;
   b.printAddr();

   cout << "after" << endl;
   {
      State a;
      a.init();

      b = a;
      cout << &a << endl;
      a.printAddr();
   }

   cout << &b << endl;
   b.printAddr();


   return 0;
}
