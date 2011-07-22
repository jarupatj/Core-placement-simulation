#include <iostream>

#include "State.h"

using namespace std;

int main(int argc, char *argv[]) {
   State b;

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
