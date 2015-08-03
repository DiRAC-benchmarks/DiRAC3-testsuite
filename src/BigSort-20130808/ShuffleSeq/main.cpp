#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <set>
using namespace std;

#define DEBUG

/*
 * Shuffle a sequence of numbers in place
 * Treat the sequence as blocks of size BLK bytes
 * The shuffle can take place in as many stages as possible. In each stage does the following
 * 1. each rank randomly picks N blocks, each block is of size BLK. Their total size fit in the DRAM
 * 2. do a collective, so rank 0 knows which blocks are handled by which rank
 * 3. rank 0 arbitrates conflict ranks, and send a message to stop conflicted ranks from shuffle at this stage
 * 4. each conflict-free rank shuffles the N blocks and write to the file
 */

char USAGE[300] = "binary <number> <dram_alloc> <file_name> <blks_per_rank> <stages> # Shuffle a binary file with integer values from 0 to number in ascending order";

#define uint64_t unsigned long long
#define TYPE uint64_t

// The mount of DRAM to allocate data 
// #define DRAM_ALLOC 1073741824L 
// #define DRAM_ALLOC 536870912L
//
void randBlocks(uint64_t* blk_ids, uint64_t blks_per_rank, uint64_t num_blks)
{
      for (uint64_t n = 0; n < blks_per_rank; n++)
      {
         uint64_t blk_id;
         int duplicate = 0;
         do{
           blk_id = rand()%num_blks;
           duplicate = 0;
           // make sure this blk_id doesn't occur before
           for (uint64_t t = 0; t < n; t++)
           {
             if(blk_ids[t] == blk_id)
             {
               // a duplicate, keep probing
               duplicate = 1;
               break;
             }
           }
         }while(duplicate);
         blk_ids[n] = blk_id;
      }
}

// Shuffle an array 
void shuffle(TYPE *array, uint64_t n)
{
    if (n > 1) 
    {
       uint64_t i;
        for (i = 0; i < n - 1; i++) 
        {
          uint64_t j = i + (rand()%(n - i));
          TYPE t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

bool isSorted(TYPE* buf, uint64_t len)
{
    bool sorted = true;
    for(uint64_t n = 0; n < len-1; n++)
    {
      if (buf[n] > buf[n+1]){
        sorted = false;
        break;
      }
    }
    return sorted;
}

void shuffleBlks(TYPE* buf, uint64_t* blk_ids, MPI_File fh, uint64_t lnum, uint64_t blk_num, uint64_t blks_per_rank)
{
    MPI_Status status;
    // read the blks into a contiguous buffer
    for(uint64_t n = 0; n < blks_per_rank; n++)
    {
      uint64_t offset = blk_ids[n] * blk_num * sizeof(TYPE);
      MPI_File_read_at( fh, offset, (void *) &buf[n*blk_num], blk_num * sizeof(TYPE), MPI_CHAR, &status);
    }
    // shuffle the buffer
    shuffle(buf, blks_per_rank * blk_num);

#ifdef DEBUG
    if (isSorted(buf, blk_num*blks_per_rank)) {
      printf("Failed to shuffle %llu x %llu elments!: %llu, %llu, %llu\n", blk_num, blks_per_rank, buf[0], buf[1], buf[2]);
    } else {
      printf("Succeeded to shuffle %llu x %llu elments!: %llu, %llu, %llu\n", blk_num, blks_per_rank, buf[0], buf[1], buf[2]);
    }
#endif

    // write the contiguous buffer to numerous scattered blocks
    for(uint64_t n = 0; n < blks_per_rank; n++)
    {
      uint64_t offset = blk_ids[n] * blk_num * sizeof(TYPE);
      MPI_File_write_at( fh, offset, (void *) &buf[n*blk_num], blk_num * sizeof(TYPE), MPI_CHAR, &status);
    }
}

bool verifyShuffle(TYPE* buf, uint64_t* blk_ids, MPI_File fh, uint64_t lnum, uint64_t blk_num, uint64_t blks_per_rank, int rank)
{
    MPI_Status status;
    // read the blks into a contiguous buffer
    for(uint64_t n = 0; n < blks_per_rank; n++)
    {
      uint64_t offset = blk_ids[n] * blk_num * sizeof(TYPE);
      MPI_File_read_at( fh, offset, (void *) &buf[n*blk_num], blk_num * sizeof(TYPE), MPI_CHAR, &status);
      if(isSorted(&buf[n*blk_num], blk_num))
        return false;
    }
    return true;
}

int main( int argc, char **argv )
{
  int rank, nprocs, ierr=0;
  double tm1, tm2;

  // - Set up MPI
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &nprocs);

  // initialize random number generator
  // (must be different for each rank)
  srand(rank);

  // - Read from input
  if (argc != 6)
  {
      if (!rank)
        printf("%s\n", USAGE);
      return -1;
  }
  // the maximum number of integer to generate
  uint64_t lnum = atol(argv[1]);
  uint64_t DRAM_ALLOC = atol(argv[2]); 
  char* FileName = argv[3];
  uint64_t blks_per_rank = atol(argv[4]);
  uint64_t num_stages = atol(argv[5]);

  if(lnum % nprocs != 0)
  {
      if (!rank)
        printf("Number of integers must be dividable by the number of ranks.\n");
      MPI_Abort(MPI_COMM_WORLD, ierr);
      return -1;
  }
  if (!rank)
      printf( "Writing %llu numbers to %s\n", lnum, FileName);

  // - Preparation
  // the total size of data in bytes
  uint64_t lsize = lnum * sizeof(TYPE);
  // bytes for each block
  if(DRAM_ALLOC % (blks_per_rank*sizeof(TYPE)) != 0)
  {
      if (!rank)
        printf("The total size in bytes for all blocks within a rank (blks_per_rank*%lu) must be dividable by the specified DRAM allocation.\n", sizeof(TYPE));
      MPI_Abort(MPI_COMM_WORLD, ierr);
      return -1;
  }
  uint64_t blk_size = DRAM_ALLOC/blks_per_rank;
  // number of elements per block
  uint64_t blk_num = blk_size/sizeof(TYPE);
  if(blk_num == 0)
  {
      if (!rank)
        printf("Block size is too small to shuffle elements");
      MPI_Abort(MPI_COMM_WORLD, ierr);
      return -1;
  }
  // total number of blocks
  if(lsize % blk_size != 0)
  {
      if (!rank)
        printf("Total number of elements (number=%llu) should be dividable by the elements per each block (dram_alloc/blks_per_rank/%lu)\n", lnum, sizeof(TYPE));
      MPI_Abort(MPI_COMM_WORLD, ierr);
      return -1;
  }
  // total number of blocks
  uint64_t num_blks = lsize/blk_size;
  
  if (!rank) printf("Ranks = %d, Blocks per rank = %llu, Block size(B) = %llu, Numbers per block = %llu, DRAM_alloc(B) = %llu\n", nprocs, blks_per_rank, blk_size, blk_num, DRAM_ALLOC);

  // allocate ram space 
  TYPE* buf = (TYPE *)malloc(DRAM_ALLOC);
  uint64_t* blk_ids = (uint64_t*)malloc(blks_per_rank * sizeof(uint64_t));
  int proceed = 0;

  // - Prepare files
  MPI_File fh;
  MPI_Info info;
  MPI_Status status;
  // synchroneous open a file
  MPI_Info_create(&info);
  MPI_File_open( MPI_COMM_WORLD, (char*) FileName, MPI_MODE_RDWR, info, &fh );
  // truncate if file existed
  // MPI_File_set_size( fh, (MPI_Offset)lsize );

  tm1 = MPI_Wtime();
  MPI_Barrier( MPI_COMM_WORLD );
  // - Start shuffle, proceed in stages
  for (uint64_t s = 0; s < num_stages; s++)
  {
      // 1. identify blocks to shuffle
      randBlocks(blk_ids, blks_per_rank, num_blks);

      if (rank)
      {
          // 2. send this to rank 0
          MPI_Send((void*)blk_ids, blks_per_rank * sizeof(uint64_t), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
          // 3. receive from rank 0 and see whether or not to proceed
          MPI_Recv(&proceed, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
      } else {
          uint64_t* rank_blk_ids = (uint64_t*)malloc(blks_per_rank * sizeof(uint64_t));
          set<uint64_t> glb_blk_ids;
          for(int r = 1; r < nprocs; r++)
          {
            // 2. receive blks proposed by each rank
            MPI_Recv((void*)rank_blk_ids, blks_per_rank * sizeof(uint64_t), MPI_CHAR, r, 0, MPI_COMM_WORLD, &status);
            int rank_proceed = 1;
            for(uint64_t i=0; i < blks_per_rank; i++)
            {
              if(glb_blk_ids.find(rank_blk_ids[i]) != glb_blk_ids.end())
                rank_proceed = 0; // conflicted
            }
            if (rank_proceed)
            { // register the blocks
              for(uint64_t i=0; i < blks_per_rank; i++)
                glb_blk_ids.insert(rank_blk_ids[i]);
            }
            // 3. send proceed commands to other ranks
            MPI_Send(&rank_proceed, 1, MPI_INT, r, 1, MPI_COMM_WORLD);
          }
          glb_blk_ids.clear();
          free(rank_blk_ids);
          proceed = 1;
      }

      MPI_Barrier( MPI_COMM_WORLD );
      if(!rank) printf("Rank %d, proceed = %d\n", rank, proceed);
      if(proceed)
      {
         // 4. shuffle the blocks and write to the file
         shuffleBlks(buf, blk_ids, fh, lnum, blk_num, blks_per_rank);
#ifdef DEBUG
         bool success = verifyShuffle(buf, blk_ids, fh, lnum, blk_num, blks_per_rank, rank);
         if(!rank) printf("Rank %d: shuffled = %d\n", rank, success);
#endif
      }
      MPI_Barrier( MPI_COMM_WORLD );
      if(!rank) printf("Rank %d, finishing stage %llu out of %llu\n", rank, s, num_stages);
  }

  // - Finishing up
  MPI_Barrier( MPI_COMM_WORLD );
  tm2 = MPI_Wtime();
  MPI_File_close(&fh);

  if(!rank) printf("Rank %d finished\n", rank);

  if ( !rank )
      printf( "Total(B) = %llu, Time(s) = %f, Bandwidth (MB/s) = %lf\n", lsize, tm2 - tm1, (double) lsize / (tm2 - tm1 ) / 1048576. );
  
  // - Finishing up
  free(buf);
  free(blk_ids);
  MPI_Finalize();
  return 0;
}

