#include <iostream>
#include <cstdlib>
#include <cstring>

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
   cout << "\nusage: ./mpiJob [options] input_file\n\n"
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

int main(int argc, char* argv[]) {

   int numProcess, rank;
   int i, c;
   double param[SIZE];
   char sendMsg[MSG_SIZE];
   char recvMsg[MSG_SIZE];

   unsigned int seed = time(NULL);
   double start = S_TEMP;
   double end = E_TEMP;
   double rate = RATE;
   int iter = ITER;
   int reject = REJECT;
   int accept = ACCEPT;
   bool verbose = false;
   bool quiet = true;
   char* inputfile;

   MPI_Status status;
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
         return 0;
      case '?':
         cout << "Unknown arguments\n";
         printUsage();
         break;
      }
   }

   inputfile = argv[optind];

   if (rank == ROOT) { //root process
      /*
       * create array to store different a, b, g, d values
       */
      double parameters[3][SIZE] = { { 0, 1, 0.2, 0.04 },
            { 0.5, 1, 0.2, 0.04 }, { 1, 1, 0.2, 0.04 } };
      /*
       * use MPI_send to send array of a,b,g,d to each child process
       */
      for (i = 1; i < numProcess; i++) {
         MPI_Send(&parameters[i - 1][0], 1, paramType, i, INPUT, MPI_COMM_WORLD);
      }
      /*
       * use MPI_Recv to receive output from child process
       */
      for (i = 1; i < numProcess; i++) {
         MPI_Recv(&recvMsg, MSG_SIZE, MPI_CHAR, i, OUTPUT, MPI_COMM_WORLD,
               &status);
         cout << "# Process " << i << endl;
         cout << recvMsg;
      }

   } else { //child process
      /*
       * receive parameters setting from root process
       */
      MPI_Recv(param, SIZE, MPI_DOUBLE, ROOT, INPUT, MPI_COMM_WORLD, &status);

      stringstream s;

      SimulatedAnnealing sa;
      int err = sa.init(param[0], param[1], param[2], param[3], start, end,
            rate, iter, reject, accept, inputfile, verbose, quiet);

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
         s << param[0] << " " << param[1] << " " << param[2] << " " << param[3]
               << " " << start << " " << end << " " << rate << " ";
         s << sa.printFinalCost();

      }
      /*
       * send result to root process
       */
      size_t length = s.str().copy(sendMsg, s.str().length(), 0);
      sendMsg[length] = '\0';
      MPI_Send(&sendMsg, MSG_SIZE, MPI_CHAR, ROOT, OUTPUT, MPI_COMM_WORLD);
   }

   MPI_Type_free(&paramType);
   MPI_Finalize();
   return 0;
}
