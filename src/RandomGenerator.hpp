#ifndef RANDOM_H
#define RANDOM_H

class RandomGenerator {
   public:
      RandomGenerator();
      ~RandomGenerator();

      //generate random number (0,...,1)
      double uniform_0_1();

      //generate random number between [0,...,n-1]
      int uniform_n(int n);

   private:

};

#endif
