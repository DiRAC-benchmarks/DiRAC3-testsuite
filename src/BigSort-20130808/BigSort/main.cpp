/*
 * Sort a big array
 * Sorting a large array of integer values
 * 1. values uniformly fall within a range, denoted by a lower bound and an upper bound.
 * 2. the total data set size is larger than the accumulated memory of all available nodes
 */

#include "globals.h"
#include "extsort.h"

char USAGE[300] = "binary <number> <dram_alloc> <file_name> <out_file> <temp_dir> <page_size>";

int main( int argc, char **argv )
{
  int rank, nprocs, ierr=0;
  double tm1, tm2, tm3, tm4;

  // - Set up MPI
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &nprocs);

  PROF_REPORT_MEM("after MPI_Init", rank);

  // - Read from input
  if (argc != 7)
  {
      if (!rank)
        printf("%s\n", USAGE);
      return -1;
  }

  // the maximum number of integer to generate
  uint64_t lnum = atol(argv[1]);
  uint64_t DRAM_ALLOC = atol(argv[2]); 
  char* FileName = argv[3];
  char* SortFileName = argv[4];
  char* TEMP_DIR = argv[5];
  uint64_t page_size = atol(argv[6]); 
  uint64_t lsize = lnum * sizeof(TYPE);

  if(DRAM_ALLOC < page_size * 2)
  {
     fprintf(stderr, "DRAM allocation must be no smaller than PAGE_SIZE * 2");
     return -1;
  }

  if(lnum % nprocs != 0)
  {
      if (!rank)
        printf("Number of integers must be dividable by the number of ranks.\n");
      MPI_Abort( MPI_COMM_WORLD, ierr );
      return -1;
  }
  if (!rank)
      printf( "Sorting %llu numbers from %s\n", lnum, FileName);

  // - Preparation
  if (!rank) printf("Ranks = %d, DRAM_alloc(B) = %llu\n", nprocs, DRAM_ALLOC);

  // - Binning
  PROF_REPORT_MEM("before Binning", rank);
  tm1 = MPI_Wtime();
  bining(NULL, TEMP_DIR, FileName, lnum, nprocs, rank, DRAM_ALLOC);
  tm2 = MPI_Wtime();
  PROF_REPORT_MEM("after Binning", rank);

#ifdef DEBUG
  PROF_REPORT_MEM("before VerifyBin", rank);
  verifyBins(NULL, TEMP_DIR, FileName, lnum, nprocs, rank, DRAM_ALLOC);
  PROF_REPORT_MEM("after VerifyBin", rank);
#endif

  if (!rank) printf("Rank %d finished Binning\n", rank);

  // - Per-node external sort
  PROF_REPORT_MEM("before ExtSort", rank);
  tm3 = MPI_Wtime();
  extSortAll(SortFileName, TEMP_DIR, lnum, nprocs, rank, DRAM_ALLOC, page_size);
  tm4 = MPI_Wtime();
  PROF_REPORT_MEM("before ExtSort", rank);

  // - Finishing up
  if (!rank) printf("Rank %d finished Sorting\n", rank);

  if ( !rank ){
      printf( "Total Data(B) = %llu\nTotal(s) = %f\nBinning Time(s) = %f\nExternal Sort Time(s) = %f\n", lsize, tm4 - tm1, tm2 - tm1, tm4 - tm3);
      iotime();
  }


  if ( !rank )
      printf( "Start verification...\n" );
  checkOrder(SortFileName, lnum, VERIFY_ALLOC, nprocs, rank);

  MPI_Finalize();

  PROF_REPORT_MEM("the end", rank);

  return 0;
}

