#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Defs.hpp"
#include "SimulatedAnnealing.hpp"

using namespace std;

void printUsage() {
   cout << "\nusage: ./sa [options] input_file\n\n"
   << "option lists are:\n"
   << "\t-a <value> : setting alpha value\n"
   << "\t-b <value> : setting beta value\n"
   << "\t-g <value> : setting gamma value\n"
   << "\t-d <value> : setting delta value\n"
   << "\t-s <value> : setting initial temperature\n"
   << "\t-e <value> : setting final threshold temperature\n"
   << "\t-r <value> : setting temperature reduction rate\n"
   << "\t-t <value> : setting iterations per temperature\n"
   << "\t-c <value> : setting number of consecutive rejection per temperature\n"
   << "\t-h         : printint usage\n\n"; 
}

int main(int argc, char* argv[]) {

   unsigned int seed = time(NULL);
   srand(seed);
   //srand(10);

   int c;
   double alpha = ALPHA;  
   double beta = BETA; 
   double gamma = GAMMA;
   double delta = DELTA;
   double start = S_TEMP; 
   double end = E_TEMP; 
   double rate = RATE; 
   int iter = ITER;
   int reject = REJECT;
   bool verbose = false;
   char* inputfile;

   if(argc == 1) {
      printUsage();
      return 0;
   }

   while( (c = getopt(argc, argv, "a:b:g:d:s:e:r:i:c:hv")) != -1) {
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
         case 'd':
            delta = atof(optarg);
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
         case 'v':
            verbose = true;
            break;
         case 'c':
            reject = atoi(optarg);
            break;
         case 'h':
            printUsage();
            break;
         case '?':
            cout << "Unknow arguments\n";
            printUsage();
            break;
      }
   }

   inputfile = argv[optind];

   SimulatedAnnealing sa;
   int err = sa.init(alpha, beta, gamma, delta, start, end, rate, iter, reject, inputfile, verbose);
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

   cout << "Random number seed " << seed << endl;
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
