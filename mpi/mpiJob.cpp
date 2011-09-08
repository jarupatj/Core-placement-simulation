#include <iostream>
#include <cstdlib>
#include <stdio.h>

#include <mpi.h>

#include "../src/Defs.hpp"
#include "../src/SimulatedAnnealing.hpp"

#define SIZE 4
#define INPUT 1
#define OUTPUT 2
#define MSG_SIZE 150
#define ROOT 0

using namespace std;

int main(int argc, char* argv[]) {

   int numProcess, rank;
   int i;
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
   char* inputfile = argv[1];

   MPI_Status status;
   MPI_Datatype paramType;

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

   MPI_Type_contiguous(SIZE, MPI_DOUBLE, &paramType);
   MPI_Type_commit(&paramType);

   srand(seed);

   if (rank == ROOT) { //root process
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
         MPI_Send(&parameters[i-1][0], 1, paramType, i, INPUT, MPI_COMM_WORLD);
      }
      /*
       * use MPI_Recv to receive output from child process
       */
      for (i = 1; i < numProcess; i++) {
         MPI_Recv(&recvMsg, MSG_SIZE, MPI_CHAR, i, OUTPUT, MPI_COMM_WORLD, &status);
         cout << "process #" << i << endl;
         cout << recvMsg;
      }

   } else { //child process
      /*
       * receive parameters setting from root process
       */
      MPI_Recv(param, SIZE, MPI_DOUBLE, ROOT, INPUT, MPI_COMM_WORLD, &status);

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

      /*
       * quiet printing
       * - print seed, parameters and cost
       */
      stringstream s;
      s << seed << " ";
      s << param[0] << " " << param[1] << " " << param[2] << " " << param[3] << " " << start
            << " " << end << " " << rate << " ";
      s << sa.printFinalCost();

      size_t length = s.str().copy(sendMsg,s.str().length(), 0);
      sendMsg[length] = '\0';

      /*
       * send result to root process
       */
      MPI_Send(&sendMsg, MSG_SIZE, MPI_CHAR, ROOT, OUTPUT, MPI_COMM_WORLD);
   }

   MPI_Type_free(&paramType);
   MPI_Finalize();
   return 0;
}
