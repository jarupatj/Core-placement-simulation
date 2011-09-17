#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>

#include <mpi.h>

#include "../src/Defs.hpp"
#include "../src/SimulatedAnnealing.hpp"

#define SIZE 4
#define INPUT 1
#define OUTPUT 2
#define MSG_SIZE 150
#define ROOT 0

using namespace std;

void printUsage() {
   cout << "\nusage: ./mpiJob [options] confic_file input_file\n\n"
         << "config file is for setting alpha, beta, gamma and delta\n"
         << "input file is for setting cores initial position\n\n"
         << "option lists are:\n"
         << "\t-s <value> : setting initial temperature (default = 1000)\n"
         << "\t-e <value> : setting final threshold temperature (default = 0.1)\n"
         << "\t-r <value> : setting temperature reduction rate (default = 0.9)\n"
         << "\t-t <value> : setting iterations per temperature (default = 400)\n"
         << "\t-c <value> : setting number of consecutive rejection per temperature (default = 200)\n"
         << "\t-p <value> : setting threshold of state accept per temperature (default = 100)\n"
         << "\t-n <value> : setting seed value for random number\n"
         << "\t-h         : print usage\n\n";
}

void root_process(char *configFile, int numProcess, MPI_Datatype & paramType) {
   MPI_Status status;
   char recvMsg[MSG_SIZE];
   ifstream file(configFile);
   double aS, aE, aI, bS, bE, bI, gS, gE, gI, dS, dE, dI;

   file >> aS >> aE >> aI >> bS >> bE >> bI >> gS >> gE >> gI >> dS >> dE >> dI;

   file.close();

   //alpha must be between [0,1]
   assert(aS >= 0 && aE <= 1);

   /*
    * create array to store different a, b, g, d values
    */
   vector<vector<double> > parameters;
   vector<double> t(4);
   for (double i = aS; i <= aE; i += aI) {
      t[0] = i;
      for (double j = bS; j <= bE; j += bI) {
         t[1] = j;
         for (double k = gS; k <= gE; k += gI) {
            t[2] = k;
            for (double l = dS; l <= dE; l += dI) {
               t[3] = l;
               parameters.push_back(t);
            }
         }
      }
   }
   int numJob = parameters.size();
   int jobCount = 0;
   int minJob = 0;
   while (jobCount < numJob) {
      //min of (number of process we have (not including root) or job left)
      minJob = min(numProcess - 1, numJob - jobCount);
      /*
       * use MPI_send to send array of a,b,g,d to each child process
       */
      for (int i = 1; i <= minJob; i++) {
         MPI_Send(&parameters[jobCount][0], 1, paramType, i, INPUT,
               MPI_COMM_WORLD);
         jobCount++;
      }
      /*
       * use MPI_Recv to receive output from child process
       */
      for (int i = 1; i <= minJob; i++) {
         MPI_Recv(&recvMsg, MSG_SIZE, MPI_CHAR, i, OUTPUT, MPI_COMM_WORLD,
               &status);
         cout << "# Process " << i << endl;
         cout << recvMsg;
      }
   }
   /*
    * send message to stop all processes
    * use alpha = -1 as an indicator to end the child process
    */
   double endParam[SIZE] = { -1, 0, 0, 0 };
   for (int i = 1; i < numProcess; i++) {
      MPI_Send(&endParam, 1, paramType, i, INPUT, MPI_COMM_WORLD);
   }
}

void child_process(double start, double end, double rate, int iter, int reject,
      int accept, char *inputFile, bool verbose, bool quiet, unsigned int seed) {
   MPI_Status status;
   char sendMsg[MSG_SIZE];
   double param[SIZE];
   /*
    * receive parameters setting from root process
    */
   MPI_Recv(param, SIZE, MPI_DOUBLE, ROOT, INPUT, MPI_COMM_WORLD, &status);

   while (param[0] != -1) {
      stringstream s;
      SimulatedAnnealing sa;
      int err = sa.init(param[0], param[1], param[2], param[3], start, end,
            rate, iter, reject, accept, inputFile, verbose, quiet);
      if (err == FILE_OPEN_ERR) {
         s << "# File open error exit" << endl;
      } else if (err == ILLEGAL_STATE_ERR) {
         s << "# Illegal initial state" << endl;
      } else {
         /*
          * start simulated annealing
          */
         sa.run();

         /*
          * quiet printing
          * - print seed, parameters and cost
          */

         s << seed << " ";
         s << setw(3) << param[0] << setw(5) << param[1] << setw(5) << param[2] << setw(5) << param[3]
               << setw(7) << start << setw(7) << end << setw(7) << rate;
         s << sa.printFinalCost();

      }
      /*
       * send result to root process
       */
      size_t length = s.str().copy(sendMsg, s.str().length(), 0);
      sendMsg[length] = '\0';
      MPI_Send(&sendMsg, MSG_SIZE, MPI_CHAR, ROOT, OUTPUT, MPI_COMM_WORLD);

      MPI_Recv(param, SIZE, MPI_DOUBLE, ROOT, INPUT, MPI_COMM_WORLD, &status);
   }
}

int main(int argc, char* argv[]) {

   int numProcess, rank;
   int c;

   unsigned int seed = time(NULL);
   double start = S_TEMP;
   double end = E_TEMP;
   double rate = RATE;
   int iter = ITER;
   int reject = REJECT;
   int accept = ACCEPT;
   bool verbose = false;
   bool quiet = true;
   char *inputFile, *configFile;

   MPI_Datatype paramType;

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

   MPI_Type_contiguous(SIZE, MPI_DOUBLE, &paramType);
   MPI_Type_commit(&paramType);

   srand(seed);

   while ((c = getopt(argc, argv, "s:e:r:i:c:p:n:h")) != -1) {
      switch (c) {
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
      case 'h':
         printUsage();
         MPI_Type_free(&paramType);
         MPI_Finalize();
         return 0;
      case '?':
         cout << "Unknown arguments\n";
         printUsage();
         break;
      }
   }

   configFile = argv[optind];
   inputFile = argv[optind + 1];

   if (rank == ROOT) {
      root_process(configFile, numProcess, paramType);
   } else {
      child_process(start, end, rate, iter, reject, accept, inputFile, verbose,
            quiet, seed);
   }

   MPI_Type_free(&paramType);
   MPI_Finalize();
   return 0;
}
