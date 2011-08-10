#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Defs.hpp"
#include "SimulatedAnnealing.hpp"

using namespace std;

int main(int argc, char* argv[]) {

   //srand(time(NULL));
   srand(10);

   if (argc != 10) {
      cout << "usage ./sa <alpha> <beta> <gamma> <theta> <starting_temp> "
            "<ending_temp> <cooling rate> <iteration/temp> <input file>" << endl;
      return 0;
   }

   SimulatedAnnealing sa;
   int err = sa.init(argv);
   if(err == FILE_OPEN_ERR) {
      cout << "File open Error Exit" << endl;
      return 0;
   } else if(err == ILLEGAL_STATE_ERR) {
      cout << "Illegal initial state" << endl;
      return 0;
   }

   //output file name is inputfile.out
   string outfile(argv[9]);
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
