#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Defs.hpp"
#include "SimulatedAnnealing.hpp"

using namespace std;

void printUsage() {
   cout << "\nusage: ./sa [options] input_file [output_file]\n\n"
   << "option lists are:\n"
   << "\t-a <value> : setting alpha value (default = 1)\n"
   << "\t-b <value> : setting beta value (default = 1)\n"
   << "\t-g <value> : setting gamma value (default = 0.2)\n"
   << "\t-d <value> : setting delta value (default = 0.04)\n"
   << "\t-s <value> : setting initial temperature (default = 1000)\n"
   << "\t-e <value> : setting final threshold temperature (default = 0.1)\n"
   << "\t-r <value> : setting temperature reduction rate (default = 0.9)\n"
   << "\t-t <value> : setting iterations per temperature (default = 400)\n"
   << "\t-c <value> : setting number of consecutive rejection per temperature (default = 200)\n"
   << "\t-p <value> : setting threshold of state accept per temperature (default = 100)\n"
   << "\t-n <value> : setting seed value for random number\n" 
   << "\t-v         : verbose printing\n"
   << "\t-h         : print usage\n\n"
   << "If no output_file, then the default output_file is 'input_file.out'\n\n";
}

int main(int argc, char* argv[]) {

   unsigned int seed = time(NULL);
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
   int accept = ACCEPT;
   bool verbose = false;
   char* inputfile = NULL;
   string outfile;

   if(argc == 1) {
      printUsage();
      return 0;
   }

   while( (c = getopt(argc, argv, "a:b:g:d:s:e:r:i:c:p:n:hv")) != -1) {
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
         case 'c':
            reject = atoi(optarg);
            break;
         case 'p':
            accept = atoi(optarg);
            break;
         case 'n':
            seed = (unsigned int) atoi(optarg);
            break;
         case 'v':
            verbose = true;
            break;
         case 'h':
            printUsage();
            break;
         case '?':
            cout << "Unknown arguments\n";
            printUsage();
            break;
      }
   }

   inputfile = argv[optind];

   if(optind+1 < argc) {
      outfile = argv[optind+1];
   } else {
      outfile = inputfile;
      outfile += ".out";

   }

   srand(seed);
   //srand(10);

   SimulatedAnnealing sa;
   int err = sa.init(alpha, beta, gamma, delta, start, end, rate, iter, \
                     reject, accept, inputfile, verbose);
   if(err == FILE_OPEN_ERR) {
      cout << "# File open error exit" << endl;
      return 0;
   } else if(err == ILLEGAL_STATE_ERR) {
      cout << "# Illegal initial state" << endl;
      return 0;
   }


   streambuf* cout_buf = cout.rdbuf(); //save original buf
   ofstream fout(outfile.c_str());
   cout.rdbuf(fout.rdbuf()); //redirect cout to the file

   cout << "# Random number seed " << seed << endl;
   cout << "# Initial State" << endl;
   sa.printSummary();
   cout << "#" << endl;
   sa.initTable();

   sa.run();

   cout << endl;
   sa.printSummary();

   cout.rdbuf(cout_buf); //restore cout original buf

   return 0;
}
