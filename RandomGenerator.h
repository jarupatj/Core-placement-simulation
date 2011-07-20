#pragma once

class RandomGenerator {
   public:
      RandomGenerator();
      ~RandomGenerator();

      //generate random number [0,...,1]
      double uniform_0_1();

      //generate random number between [1,...,n]
      int uniform_n(int n);

   private:

};
