#include <iostream>

#include "RandomGenerator.hpp"

using namespace std;

int main() {
   RandomGenerator r;
   int f[5] = {0};
   int f2[12] = {0};
   double num;
   for(unsigned int i = 0; i < 3000000; i++) {
      num = r.uniform_0_1();
      if (num == 0.0) f2[11]++;
      else if (num == 1.0) f2[10]++;
      else if (num < 0.1) f2[0]++;
      else if (num < 0.2) f2[1]++;
      else if (num < 0.3) f2[2]++;
      else if (num < 0.4) f2[3]++;
      else if (num < 0.5) f2[4]++;
      else if (num < 0.6) f2[5]++;
      else if (num < 0.7) f2[6]++;
      else if (num < 0.8) f2[7]++;
      else if (num < 0.9) f2[8]++;
      else if (num < 1) f2[9]++;
   }
   cout << "0-1" << endl;
   for(int i = 0; i < 12; i++) {
      cout << i << ": " << f2[i] << endl;
   }

   for(int i = 0; i < 1000; i++) {
      f[r.uniform_n(4)]++;
   }
   cout << "n" << endl;
   for(int i = 0; i < 5; i++) {
      cout << i << ": " << f[i] << endl;
   }
   return 0;
}
