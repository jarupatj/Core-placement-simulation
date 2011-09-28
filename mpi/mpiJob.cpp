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
#include "../src/Simulator.hpp"

#define SIZE 4
#define INPUT 1
#define OUTPUT 2
#define SEED 3
#define MSG_SIZE 150
#define ROOT 0
#define ALPHA_INDEX 0
#define BETA_INDEX 1
#define GAMMA_INDEX 2
#define DELTA_INDEX 3

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

bool ratio_printing = false;

void root_process(char *configFile, int numProcess, MPI_Datatype & paramType) {
   MPI_Status status;
   char recvMsg[MSG_SIZE];
   unsigned int r;

   srand(time(NULL));

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
   vector<double> t(SIZE);
   for (double i = aS; i <= aE; i += aI) {
      t[ALPHA_INDEX] = i;
      for (double j = bS; j <= bE; j += bI) {
         t[BETA_INDEX] = j;
         for (double k = gS; k <= gE; k += gI) {
            t[GAMMA_INDEX] = k;
            for (double l = dS; l <= dE; l += dI) {
               t[DELTA_INDEX] = l;
               parameters.push_back(t);
            }
         }
      }
   }
   int numJob = parameters.size(); /* total number of jobs that need to be done */
   int jobCount = 0; /* number of jobs that we've processed */
   int minJob = 0;
   int numRunning = 0; /* count number of process running */
   /*
    * Calculate how many children processes are needed.
    * Minimum number of children processes needed is the mininum between
    * number of available child process and number of jobs left
    */
   minJob = min(numProcess - 1, numJob - jobCount);
   /*
    * use MPI_send to send array of a,b,g,d to each child process
    */
   for (int i = 1; i <= minJob; i++) {
      MPI_Send(&parameters[jobCount][0], 1, paramType, i, INPUT, MPI_COMM_WORLD);
      r = (unsigned int) rand();
      MPI_Send(&r, 1, MPI_UNSIGNED, i, SEED, MPI_COMM_WORLD);
      jobCount++;
      numRunning++;
   }

   /*
    * If there's still at least one process running then we need to
    * wait until we received the result of that process
    */
   while (numRunning != 0) {
      /*
       * use MPI_Recv to receive output from child process
       */
      MPI_Recv(&recvMsg, MSG_SIZE, MPI_CHAR, MPI_ANY_SOURCE, OUTPUT,
            MPI_COMM_WORLD, &status);
      //cout << "# Process " << status.MPI_SOURCE << endl;
      cout << recvMsg;
      numRunning--;
      /*
       * We still have more jobs to be done
       */
      if (jobCount < numJob) {
         MPI_Send(&parameters[jobCount][0], 1, paramType, status.MPI_SOURCE,
               INPUT, MPI_COMM_WORLD);
         r = (unsigned int) rand();
         MPI_Send(&r, 1, MPI_UNSIGNED, status.MPI_SOURCE, SEED, MPI_COMM_WORLD);
         jobCount++;
         numRunning++;
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
      int accept, char *inputFile, bool verbose, bool quiet) {
   MPI_Status status;
   char sendMsg[MSG_SIZE];
   double param[SIZE];
   unsigned int seed;
   double sumRatio = 0;
   int numSimulation = 1;
   if (ratio_printing) {
      numSimulation = 10;
   }

   /*
    * receive parameters setting from root process
    */
   MPI_Recv(param, SIZE, MPI_DOUBLE, ROOT, INPUT, MPI_COMM_WORLD, &status);

   /*
    * loop until receive the end indicator which is when alpha == -1
    */
   while (param[ALPHA_INDEX] != -1) {
      /*
       * receive seed setting from root process
       */
      MPI_Recv(&seed, 1, MPI_UNSIGNED, ROOT, SEED, MPI_COMM_WORLD, &status);
      /*
       * seed random number generator
       */
      srand(seed);

      sumRatio = 0;

      stringstream s;
      for (int i = 0; i < numSimulation; i++) {
         Simulator sa;
         int err = sa.init(param[ALPHA_INDEX], param[BETA_INDEX],
               param[GAMMA_INDEX], param[DELTA_INDEX], start, end, rate, iter,
               reject, accept, inputFile, verbose, quiet);
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
            if (ratio_printing) {
               sumRatio += sa.getCostRatio();
            } else {
               s << seed << "\t";
               s << setw(3) << param[ALPHA_INDEX] << setw(5)
                     << param[BETA_INDEX] << setw(5) << param[GAMMA_INDEX]
                     << setw(5) << param[DELTA_INDEX] << setw(7) << start
                     << setw(7) << end << setw(7) << rate;
               s << sa.printFinalCost();
            } /* end else */
         } /* end else */
      } /* end for */

      if (ratio_printing) {
         s << setw(3) << param[ALPHA_INDEX] << setw(5) << param[BETA_INDEX]
               << setw(5) << param[GAMMA_INDEX] << setw(5)
               << param[DELTA_INDEX] << setw(7) << setprecision(4) << sumRatio / numSimulation
               << endl;
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

   while ((c = getopt(argc, argv, "s:e:r:i:c:p:n:ha")) != -1) {
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
      case 'a':
         ratio_printing = true;
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
            quiet);
   }

   MPI_Type_free(&paramType);
   MPI_Finalize();
   return 0;
}
