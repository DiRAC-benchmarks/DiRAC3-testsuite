#include "globals.h"

bool checkOrder(char* FileName, uint64_t lnum, uint64_t DRAM_ALLOC, int nprocs, int rank)
{
  double tm1, tm2, tm3, tm4;
  if(lnum % nprocs != 0)
  {
      if (!rank)
        printf("Number of integers much be dividable by the number of ranks.\n");
      return -1;
  }
  if (!rank)
      printf( "Reading %lu numbers from %s\n", lnum, FileName);

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
  TYPE* buf = (TYPE *)malloc((num_per_ram+1) * sizeof(TYPE));

  // - Prepare files
  MPI_File fh;
  MPI_Info info;
  MPI_Status status;
  MPI_Offset offset, LENGTH, file_size;

  // - Writing files: each rank write its own chunk, each chunk is writen in phases according to DRAM_ALLOC
  uint64_t k, start, end;
  start = rank * num_per_rank;
  end = start + num_per_rank;
  end = (end > lnum)?lnum : end;

#ifdef DEBUG
  printf("Rank %d: comparing numbers in [%llu, %llu]\n", rank, start, end);
#endif

  // synchroneous open a file
  MPI_Info_create(&info);
  MPI_File_open( MPI_COMM_WORLD, (char*) FileName, MPI_MODE_RDONLY, info, &fh );

  tm1 = MPI_Wtime();
  int out_of_order = 0;
  // start validating
  MPI_Barrier( MPI_COMM_WORLD );
  for (k = start; k < end; k += num_per_ram)
  {
      // populate values in d
      uint64_t num_per_stage = end - k;
      if (num_per_stage > num_per_ram)
          num_per_stage = num_per_ram;

      // write d to the corresponding chunk
      // MPI_Barrier( MPI_COMM_WORLD );

      // each block is of the same size, we know the offset from the rank
      offset = k;
      int cmp_begin;
      if (offset > 0)
      {
        offset -= 1;
        num_per_stage += 1;
      }
      offset *= sizeof(TYPE);
      MPI_File_read_at_all( fh, offset, (void *) buf, num_per_stage * sizeof(TYPE), MPI_CHAR, &status);

#ifdef DEBUG
      // printf("Checking ordering of numbers from %lld to %lld: %llu, %llu, %llu, %llu\n", k, k+num_per_stage, buf[0], buf[1], buf[2], buf[3]);
      printf("Checking ordering of numbers from %llu to %llu\n", k, k+num_per_stage);
      printf("examples: %llu, %llu, %llu\n", buf[0], buf[1], buf[2]);
#endif

      for (uint64_t n = 0; n < num_per_stage-1; n++)
      {
          if (buf[n] > buf[n+1])
          {
              out_of_order = 1;
              break;
          }
      }
      if (out_of_order) break;
  }

#ifdef DEBUG
  printf (" Rank %d, Ordered = %d\n", rank, 1-out_of_order);
#endif

  MPI_Barrier( MPI_COMM_WORLD );
  tm2 = MPI_Wtime();

  MPI_File_close(&fh);

  if ( !rank )
      printf( "TotalChecked(B) = %llu\nCheckedTime(s) = %f\nOrdered = %d\n", lsize, tm2 - tm1, (int) 1-out_of_order );
  
  // - Finishing up
  free(buf);
  return out_of_order == 0;
}

