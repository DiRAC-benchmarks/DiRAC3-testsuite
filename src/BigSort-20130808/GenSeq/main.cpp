#include "mpi.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG

char* USAGE = "gen_seq <number> <dram_alloc> <file_name> # Generating a binary file with integer values from 0 to number in ascending order";

#define TYPE uint64_t

/* The mount of DRAM to allocate data */
// #define DRAM_ALLOC 1073741824L 
// #define DRAM_ALLOC 536870912L

int main( int argc, char **argv )
{
  int rank, nprocs, ierr=0;
  double tm1, tm2, tm3, tm4;

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

  if(lnum % nprocs != 0)
  {
      if (!rank)
        printf("Number of integers much be dividable by the number of ranks.\n");
      MPI_Abort(MPI_COMM_WORLD, ierr);
      return -1;
  }
  if (!rank)
      printf( "Writing %ld numbers to %s\n", lnum, FileName);

  // - Preparation
  // the total size of data in bytes
  uint64_t lsize = lnum * sizeof(TYPE);
  // data size per rank
  uint64_t size_per_rank = lsize/nprocs;
  // number of elements per rank
  uint64_t num_per_rank = lnum/nprocs;
  // number of elements per DRAM
  uint64_t num_per_ram = DRAM_ALLOC/sizeof(TYPE);
  if (num_per_ram > num_per_rank)
    num_per_ram = num_per_rank;
  
  if (!rank) printf("Ranks = %d, Numbers per rank = %llu, Numbers per RAM = %llu, DRAM_alloc(B) = %llu\n", nprocs, num_per_rank, num_per_ram, DRAM_ALLOC);

  // allocate ram space 
  TYPE* buf = (TYPE *)malloc(num_per_ram * sizeof(TYPE));

  // - Prepare files
  MPI_File fh;
  MPI_Info info;
  MPI_Status status;
  MPI_Offset offset, LENGTH, file_size;

  // - Writing files: each rank write its own chunk, each chunk is writen in phases according to DRAM_ALLOC
  uint64_t start, end, k, n, bytes;
  start = rank * num_per_rank;
  end = start + num_per_rank;
  if(!rank)
      printf("Rank %d: generate numbers in [%llu, %llu)\n", rank, start, end);

  // synchroneous open a file
  MPI_Info_create(&info);
  MPI_File_open( MPI_COMM_WORLD, (char*) FileName, MPI_MODE_WRONLY|MPI_MODE_CREATE, info, &fh );
  // truncate if file existed
  MPI_File_set_size( fh, (MPI_Offset)lsize );

  tm1 = MPI_Wtime();
  // start writing
  MPI_Barrier( MPI_COMM_WORLD );
  for (k = start; k < end; k += num_per_ram)
  {
      // populate values in d
      uint64_t num_per_stage = end - k;
      if (num_per_stage > num_per_ram)
          num_per_stage = num_per_ram;
      for (n = 0; n < num_per_stage; n++)
          buf[n] = n+k;
      // each block is of the same size, we know the offset from the rank
      offset = k * sizeof(TYPE);
      bytes = num_per_stage *sizeof(TYPE);
      // offset = rank * 1024; num_per_stage = 16;
      MPI_File_write_at( fh, offset, (void *)buf, bytes, MPI_CHAR, &status);
#ifdef DEBUG
      // write d to the corresponding chunk
      fprintf(stderr, "Rank %d: Writing numbers from %llu to %llu. Offset = %llu, Bytes = %llu\n", rank, k, k+num_per_stage, offset, bytes);
      fprintf(stderr, "Rank %d: Examples: %llu, %llu, %llu\n", rank, buf[0], buf[1], buf[2]);
#endif

  }
  MPI_Barrier( MPI_COMM_WORLD );
  tm2 = MPI_Wtime();
  MPI_File_close(&fh);

  if (!rank) printf("Rank %d finished\n", rank);

  if ( !rank )
      printf( "Total(B) = %llu, Time(s) = %f, Bandwidth (MB/s) = %lf\n", lsize, tm2 - tm1, (double) lsize / (tm2 - tm1 ) / 1048576. );
  
  // - Finishing up
  free(buf);
  MPI_Finalize();
  return 0;
}

