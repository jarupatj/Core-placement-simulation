#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Defs.hpp"
#include "SimulatedAnnealing.hpp"

using namespace std;

int main(int argc, char* argv[]) {

   srand(time(NULL));
   //srand(10);

   int c;
   double alpha = ALPHA;  
   double beta = BETA; 
   double gamma = GAMMA;
   double theta = THETA; 
   double start = S_TEMP; 
   double end = E_TEMP; 
   double rate = RATE; 
   int iter = ITER;
   char* inputfile;

   while( (c = getopt(argc, argv, "a:b:g:t:s:e:r:i:h")) != -1) {
      switch(c) {
         case 'a':
            alpha = atof(optarg);
            break;
         case 'b':
            beta = atof(optarg);
            break;
         case 'g':
            gamma = atof(optarg);
            break;
         case 't':
            theta = atof(optarg);
            break;
         case 's':
            start = atof(optarg);
            break;
         case 'e':
            end = atof(optarg);
            break;
         case 'r':
            rate = atof(optarg);
            break;
         case 'i':
            iter = atoi(optarg);
            break;
         case 'h':
            cout << "print usage\n";
            break;
         case '?':
            cout << "Unknow arguments\n";
            break;
      }
   }

   inputfile = argv[optind];

   SimulatedAnnealing sa;
   int err = sa.init(alpha, beta, gamma, theta, start, end, rate, iter, inputfile);
   if(err == FILE_OPEN_ERR) {
      cout << "File open error exit" << endl;
      return 0;
   } else if(err == ILLEGAL_STATE_ERR) {
      cout << "Illegal initial state" << endl;
      return 0;
   }

   //output file name is inputfile.out
   string outfile(inputfile);
   outfile += ".out";
   streambuf* cout_buf = cout.rdbuf(); //save original buf
   ofstream fout(outfile.c_str());
   cout.rdbuf(fout.rdbuf()); //redirect cout to the file

   cout << "Initial State" << endl;
   sa.printSummary();
   cout << endl;
   sa.initTable();

   sa.run();

   cout << endl;
   sa.printSummary();

   cout.rdbuf(cout_buf); //restore cout original buf

   return 0;
}
