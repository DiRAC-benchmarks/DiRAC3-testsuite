#include "globals.h"

char USAGE[300] = "gen_seq <number> <dram_alloc> <file_name> # Generating a binary file with integer values from 0 to number in ascending order";

/* The mount of DRAM to allocate data */
// #define DRAM_ALLOC 1073741824L 
// #define DRAM_ALLOC 536870912L

int main( int argc, char **argv )
{
  int rank, nprocs;

  // - Set up MPI
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &nprocs);


  // - Read from input
  if (argc != 4)
  {
      if (!rank)
        printf("%s\n", USAGE);
      return -1;
  }
  // the maximum number of integer to generate
  uint64_t lnum = atol(argv[1]);
  uint64_t DRAM_ALLOC = atol(argv[2]); 
  char* FileName = argv[3];

  bool ordered = false;
  ordered = checkOrder(FileName, lnum, DRAM_ALLOC, nprocs, rank);
  MPI_Finalize();
  return 0;
}

