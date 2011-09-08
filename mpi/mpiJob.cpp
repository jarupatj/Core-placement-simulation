#include <iostream>
#include <stdio.h>

#include <mpi.h>

#include "../src/Defs.hpp"
#include "../src/SimulatedAnnealing.hpp"

#define SIZE 4
#define TAG 1

using namespace std;

int main(int argc, char* argv[]) {

   int numProcess, rank;
   int i;
   double param[SIZE];

   unsigned int seed = time(NULL);
   double start = S_TEMP;
   double end = E_TEMP;
   double rate = RATE;
   int iter = ITER;
   int reject = REJECT;
   int accept = ACCEPT;
   bool verbose = false;
   bool quiet = true;
   char* inputfile = argv[1];

   MPI_Status status;
   MPI_Datatype paramType;

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

   MPI_Type_contiguous(SIZE, MPI_DOUBLE, &paramType);
   MPI_Type_commit(&paramType);

   if (rank == 0) { //root process
      /*
       * create array to store different a, b, g, d values
       */
      double parameters[3][SIZE] = {
            {0,   1, 0.2, 0.04},
            {0.5, 1, 0.2, 0.04},
            {1,   1, 0.2, 0.04} };
      /*
       * use MPI_send to send array of a,b,g,d to each child process
       */
      for (i = 1; i < numProcess; i++) {
         MPI_Send(&parameters[i-1][0], 1, paramType, i, TAG, MPI_COMM_WORLD);
      }

      /*
       * use MPI_Recv to receive output from child process
       */
      /*for (i = 1; i < numProcess; i++) {
         MPI_Recv();
      }

      FILE *fp = fopen("mpiJob.out");
      if (fp == NULL) {
         printf("can't open file");
      }*/

   } else { //child process

      /*
       * receive parameters setting from root process
       */
      MPI_Recv(param, SIZE, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

      SimulatedAnnealing sa;
      int err = sa.init(param[0], param[1], param[2], param[3], start, end, rate, iter,
            reject, accept, inputfile, verbose, quiet);

      if (err == FILE_OPEN_ERR) {
         cout << "# File open error exit" << endl;
         return 0;
      } else if (err == ILLEGAL_STATE_ERR) {
         cout << "# Illegal initial state" << endl;
         return 0;
      }
      /*
       * start simulated annealing
       */
      sa.run();

      cout << "proc #" << rank << endl;
      /*
       * quiet printing
       * - print seed, parameters and costs
       */
      stringstream s;
      s << seed << " ";
      s << param[0] << " " << param[1] << " " << param[2] << " " << param[3] << " " << start
            << " " << end << " " << rate << " ";
      s << sa.printFinalCost();
      cout << s.str();
   }

   MPI_Type_free(&paramType);
   MPI_Finalize();
   return 0;
}
