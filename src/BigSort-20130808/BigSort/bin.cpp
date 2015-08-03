
/* 
 * 1. Bin elements to N chunks, where N = # of nodes. In the end, each chunk serves as a bin for elements falling within a sub range
 * a. Read from shared file system
 * b. Exchange elements in stages according to DRAM allocation (MPI communication, AllToAll)
 *    In each stage, each node/rank does the following:
 *    - read the buffer, sort it in place
 *    - count how much to send to each other process in send_cnt[]
 *    - repeat the following:
 *    	- MPI_AllToAll to notify others how much data to send
 *    	- Calculate how much data can be received, acknowledge with MPI_AllToAll about how much data is acceptable. Eventually, each process got a ack_send_cnt[]
 *    	- Send and recv data according to ack_send_cnt[] (MPI_AllToAllv)
 *    	- Write the received data to the local disk copy
 *    	- Update to get what's left to send: send_cnt[:] = send_cnt[:]-ack_send_cnt[:]
 *    	- If send_cnt is completely cleared, break
 */

#include "globals.h"
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

void printArr(int* array, int num, int rank, char* note)
{
  char string[2000]="";
  for (int n = 0; n < num; n++){
    int len = strlen(string);
    snprintf(((char*)string)+len, 2000, "%d,", array[n]);
  }
  fprintf(stderr, "Rank %d: %s: %s", rank, note, string);
}

uint64_t sum(int* send_cnt, int nprocs)
{
  uint64_t ret = 0;
  for(int n=0; n < nprocs; n++)
  {
    ret += send_cnt[n];
  }
  return ret;
}

struct Bin {
  TYPE* buf;
  uint64_t len;
  Bin* next;
};
void QuickBin(TYPE* buf, uint64_t num_per_stage, uint64_t num_per_rank, int nprocs, int* send_cnt, int* send_disp, int rank)
{
  uint64_t n, id;
  Bin** bins = (Bin**) malloc(nprocs * sizeof(Bin*));
  for(n = 0; n < nprocs; n++)
    bins[n] = NULL;
  for(n = 0; n < num_per_stage; n++) {
    id = buf[n]/num_per_rank;
    if (id >= nprocs){
      fprintf(stderr, "unexpected bin: buf[%lu]=%lu, num_per_rank=%lu, id=%lu, nprocs=%d\n", n, buf[n], num_per_rank, id, nprocs);
      throw;
    }
    if (bins[id] == NULL) {
      bins[id] = (Bin*) malloc(sizeof(Bin));
      bins[id]->len = 0; bins[id]->next = NULL;
      bins[id]->buf = (TYPE*) malloc(BIN_PAGE_SIZE);
    }
    bins[id]->buf[bins[id]->len++] = buf[n];
    if (bins[id]->len == (BIN_PAGE_SIZE/sizeof(TYPE))) {
      Bin* new_bin = (Bin*) malloc(sizeof(Bin));
      new_bin->len = 0;
      new_bin->buf = (TYPE*) malloc(BIN_PAGE_SIZE);
      new_bin->next = bins[id];
      bins[id] = new_bin;
    }
  }
  //fprintf(stderr, "Rank %d: finish hash binning\n", rank);
  uint64_t buf_pos=0;
  for(n = 0; n < nprocs; n++){
    send_cnt[n] = 0;
    send_disp[n] = buf_pos*sizeof(TYPE);
    Bin* ptr = bins[n];
    Bin* next;
    while(ptr){
      // copy the bin
      memcpy(buf+buf_pos, ptr->buf, sizeof(TYPE)*ptr->len);
      buf_pos += ptr->len;
      send_cnt[n] += ptr->len;
      // free space
      free(ptr->buf);
      next = ptr->next;
      free(ptr);
      ptr = next;
    }
  }
  //fprintf(stderr, "Rank %d: finish merge binning\n", rank);
  free(bins);
}

// only accept a total amount that can be handled by the current bin
void acceptCircular(int* recv_cnt, int nprocs, uint64_t num_per_ram)
{
  // implement a circular buffer search, all ranks in recv_cnt have equal chance
  int start_cnt = rand()%nprocs;
  int i = start_cnt;
  uint64_t total = 0;
  for(int n = 0; n < nprocs; n++)
  {
    if (total + recv_cnt[i] > num_per_ram)
    { // overflow, set acceptable count to 0
      recv_cnt[i] = 0;
    } else {
      total += recv_cnt[i];
    }
    i++;
    i = (i >= nprocs) ? 0 : i;
  } 
}

// only accept a total amount that can be handled by the current bin
// reject the biggest chunks of data
// This can be buggy --- if all senders are sending the same amount of data, then it's screwed
uint64_t accept(int* recv_cnt, int nprocs, uint64_t num_per_ram)
{
  int* sorted_cnt = (int*) malloc(nprocs*sizeof(int));
  memcpy(sorted_cnt, recv_cnt, nprocs*sizeof(int));
  QuickSortInt(sorted_cnt, 0, nprocs-1);
  int bar = 0;
  uint64_t total = 0;
  for(int n = 0; n < nprocs; n++)
  {
    total += sorted_cnt[n];
    if (total > num_per_ram)
    {
      bar = sorted_cnt[n];
      break;
    }
  }
  total -= bar;
  for(int n = 0; n < nprocs; n++)
    if(recv_cnt[n] >= bar && bar > 0)
      recv_cnt[n] = 0;
  free(sorted_cnt);
  return total;
}

// This assumes numbers in buf uniformly ranges from [0:num_per_rank*nprocs)
void localBin(TYPE* buf, uint64_t num_per_stage, uint64_t num_per_rank, int nprocs, int* send_cnt, int* send_disp)
{
  uint64_t bar = num_per_rank;
  uint64_t cnt = 0;
  uint64_t n = 0;
  int head = 0;
  int disp = 0;
  while ( n < num_per_stage)
  {
    if(buf[n] < bar)
    {
      cnt++;
      n++;
    } else {
      send_cnt[head] = cnt;
      send_disp[head] = disp;
      disp += cnt * sizeof(TYPE);
      cnt = 0;
      head ++;
      bar += num_per_rank;
    }
  }
  send_cnt[head] = cnt;
  send_disp[head] = disp;
  disp += cnt * sizeof(TYPE);
  for (n = head + 1; n < nprocs; n ++)
  {
    send_cnt[n] = 0;
    send_disp[n] = disp;
  }
}


/* Parallel binning shuffle sequence of numbers in FileName to SortFileName
 * The total of lnum numbers range from 0 to lnum (exclusive)
 * If prefix is not None, then the output is a single
 * Otherwise, it's one file per rank (sortFileName.rank)
 */
void bining(const char* SortFileName, const char* TEMP_DIR, const char* FileName, uint64_t lnum, int nprocs, int rank, uint64_t DRAM_ALLOC)
{
#ifdef DEBUG
  fprintf(stderr, "Rank %d: initialization. %d ranks in total\n", rank, nprocs);
#endif 
  double tm1, tm2, io_time=0., sort_time=0., categorize_time=0.;
  uint64_t PER_BUF_ALLOC = (DRAM_ALLOC/sizeof(TYPE)/2)*sizeof(TYPE);
  // - Preparation
  // the total size of data in bytes
  uint64_t lsize = lnum * sizeof(TYPE);
  // data size per rank
  uint64_t size_per_rank = lsize/nprocs;
  // number of elements per rank
  uint64_t num_per_rank = lnum/nprocs;
  // number of elements per DRAM
  uint64_t num_per_ram = PER_BUF_ALLOC/sizeof(TYPE);
  if (num_per_ram > num_per_rank)
    num_per_ram = num_per_rank;
#ifdef DEBUG
  fprintf(stderr, "Rank %d: allocating buffers\n", rank);
#endif 
  // allocate ram space 
  TYPE* buf = (TYPE *)malloc((num_per_ram) * sizeof(TYPE));
  TYPE* bin = (TYPE *)malloc((num_per_ram) * sizeof(TYPE));
  // number of elemnts to send and recv
  int* send_cnt = (int*)malloc(nprocs * sizeof(int));
  int* recv_cnt = (int*)malloc(nprocs * sizeof(int));
  int* ack_send_cnt = (int*)malloc(nprocs * sizeof(int));
  // amount of data (B) to send and recv
  int* send_bytes = (int*)malloc(nprocs * sizeof(int));
  int* recv_bytes = (int*)malloc(nprocs * sizeof(int));
  // displacement used in MPI_AllToAllv
  int* send_disp = (int*)malloc(nprocs * sizeof(int));
  int* recv_disp = (int*)malloc(nprocs * sizeof(int));
  int mpi_err;
  // seed rand generator, different for each rank
  srand(rank);

#ifdef DEBUG
  fprintf(stderr, "Rank %d: opening input files\n", rank);
#endif 
  // - Prepare files
  MPI_File in_fh, out_fh;
  int out_fd;
  MPI_Info info;
  MPI_Status status;
  MPI_Offset offset, LENGTH, file_size;
  // synchroneous open the input file
  MPI_Info_create(&info);
  MPI_File_open( MPI_COMM_WORLD, (char*) FileName, MPI_MODE_RDONLY, info, &in_fh );

  // - Binning boundaries
  uint64_t k, start, end, wr_head, wr_start;
  start = rank * num_per_rank;
  end = start + num_per_rank;
  end = (end > lnum)?lnum : end;

#ifdef DEBUG
  fprintf(stderr, "Rank %d: opening output files\n", rank);
#endif 

  // synchroneous open the output file
  if (SortFileName != NULL)
  {
#ifdef DEBUG
  fprintf(stderr, "Rank %d: opening unified output files\n", rank);
#endif 
    MPI_Info_create(&info);
    MPI_File_open( MPI_COMM_WORLD, (char*) SortFileName, MPI_MODE_WRONLY|MPI_MODE_CREATE, info, &out_fh );
    // truncate if file existed
    MPI_File_set_size( out_fh, (MPI_Offset)lsize );
    wr_start = wr_head = start * sizeof(TYPE);
  } else {
    char filename[2000];
    snprintf(filename, 2000, "%s/%s%d", TEMP_DIR, BINFILE_PREFIX, rank);
#ifdef DEBUG
    fprintf(stderr, "Rank %d: opening individual output files: %s\n", rank, filename);
#endif 
    out_fd = safeOpen(filename, O_WRONLY|O_CREAT, 0777);
#ifdef DEBUG
  fprintf(stderr, "Rank %d: opened %s\n", rank, filename);
#endif 
    // truncate if file existed
    wr_start = wr_head = 0;
  }
  
#ifdef DEBUG
  fprintf(stderr, "Rank %d: start binning\n", rank);
#endif 

  MPI_Barrier( MPI_COMM_WORLD );

  for (k = start; k < end; k += num_per_ram)
  {
      if(!rank) printf("Rank = %d: Binning [%lu, %lu) within [%lu, %lu)", rank, k, k+num_per_ram, start, end);
      // load values to buf
      uint64_t num_per_stage = end - k;
      if (num_per_stage > num_per_ram)
          num_per_stage = num_per_ram;

#ifdef DEBUG
      // write d to the corresponding chunk
      // MPI_Barrier( MPI_COMM_WORLD );
      fprintf(stderr, "Rank %d: Loading numbers from %llu to %llu, towards %llu\n", rank, k, k+num_per_stage, end);
#endif

      // each block is of the same size, we know the offset from the rank
      offset = k * sizeof(TYPE);
      tm1 = MPI_Wtime();
      MPI_File_read_at( in_fh, (MPI_Offset)offset, (void *) buf, (int)num_per_stage * sizeof(TYPE), MPI_CHAR, &status);
      tm2 = MPI_Wtime();
      io_time += tm2-tm1;

#ifdef DEBUG
      fprintf(stderr, "Rank %d: (%d ranks) bin the buffer locally: %llu elements.\n", rank, nprocs, num_per_stage);
#endif

      tm1 = MPI_Wtime();
      // sort each buffer
      QuickBin(buf, num_per_stage, num_per_rank, nprocs, send_cnt, send_disp, rank);
      tm2 = MPI_Wtime();
      sort_time += tm2-tm1;

#ifdef DEBUG
      fprintf(stderr, "Rank %d: start exchange\n", rank);
#endif
      int more = 0;
      int global_more;
      do {
        // probe what data to send and what to be received
#ifdef DEBUG
        printArr(send_cnt, nprocs, rank, "send_cnt");
#endif
        mpi_err = MPI_Alltoall(send_cnt,1,MPI_INT,recv_cnt,1,MPI_INT, MPI_COMM_WORLD);
#ifdef DEBUG
        printArr(send_cnt, nprocs, rank, "recv_cnt");
#endif
        // only accept a total amount that can be handled by the current bin
        acceptCircular(recv_cnt, nprocs, num_per_ram);
#ifdef DEBUG
        printArr(send_cnt, nprocs, rank, "accepted_recv_cnt");
#endif

        // acknowledge the acceptable amount
        mpi_err = MPI_Alltoall(recv_cnt,1,MPI_INT,ack_send_cnt,1,MPI_INT, MPI_COMM_WORLD);
#ifdef DEBUG
        printArr(send_cnt, nprocs, rank, "ack_send_cnt");
#endif
        // compute the recv displacement and amount of data to transfer
	for(int i=0;i<nprocs;i++)
        {
            send_bytes[i] = ack_send_cnt[i] * sizeof(TYPE);
            recv_bytes[i] = recv_cnt[i] * sizeof(TYPE);
        }
        recv_disp[0]=0;
	for(int i=1; i < nprocs; i++)
        {
            recv_disp[i]=recv_bytes[i-1]+recv_disp[i-1];
	}
        // send data according to the acceptable amount
        mpi_err = MPI_Alltoallv(buf,send_bytes,send_disp,MPI_CHAR,
				bin,recv_bytes,recv_disp,MPI_CHAR, MPI_COMM_WORLD);

        // write to output file
        uint64_t total_recv_bytes = sum(recv_bytes, nprocs);
#ifdef DEBUG
      bool check = true;
      for(int m = 0; m < total_recv_bytes/sizeof(TYPE); m++)
        if (bin[m] < start || bin[m] >= end)
        {
          check = false; break;
        }
      fprintf(stderr, "Rank %d: finish one phase of global binning. Received %lu elements. check=%d\n", rank, (unsigned long)total_recv_bytes/sizeof(TYPE), (int) check);
#endif
        tm1 = MPI_Wtime();
        if (SortFileName != NULL)
           MPI_File_write_at( out_fh, wr_head, (void*) bin, total_recv_bytes, MPI_CHAR, &status);
        else
           safeWrite(out_fd, (void*) bin, total_recv_bytes, "bin_write");
        tm2 = MPI_Wtime();
        io_time += tm2-tm1;
        wr_head += total_recv_bytes;
        // update what's left to send
        more = 0;
        for (int n = 0; n < nprocs; n ++)
        {
          send_cnt[n] -= ack_send_cnt[n];
          if (send_cnt[n] > 0)
          {
            more = 1;
#ifdef DEBUG
      fprintf(stderr, "Rank %d: %lu elements to send to rank %d. check=%d\n", rank, send_cnt[n], n);
#endif
          }
        }
        PROF_REPORT_MEM("after MPI_File_write_at_all", rank);
        MPI_Allreduce(&more, &global_more, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
        PROF_REPORT_MEM("after MPI_Allreduce", rank);
#ifdef DEBUG
      fprintf(stderr, "Rank %d: exchanged once. more = %d\n", rank, (int)global_more);
#endif
      } while (global_more > 0);
  }
  if (wr_head-wr_start != (end-start) * sizeof(TYPE))
  {
    fprintf(stderr, "Rank %d: wrote %lu numbers while expecting %lu numbers (Error)?\n", rank, (wr_head-wr_start)/sizeof(TYPE), end-start);
  }

  // - Finishing up
#ifdef DEBUG
  fprintf(stderr, "Rank %d: done\n", rank);
#endif
  MPI_Barrier( MPI_COMM_WORLD );

  if (!rank){
      printf("BinSortTime(s) = %f\nBinCategorizeTime(s) = %f\nBinIOTime(s) = %f\n", sort_time, categorize_time, io_time);
  }

  PROF_REPORT_MEM("before closing Files for Binning", rank);

  MPI_File_close(&in_fh);
  if (SortFileName != NULL)
    MPI_File_close(&out_fh);
  else
    close(out_fd);
  free(send_cnt);
  free(recv_cnt);
  free(send_bytes);
  free(recv_bytes);
  free(send_disp);
  free(recv_disp);
  free(ack_send_cnt);
  free(bin);
  free(buf);
}

void verifyBins(const char* SortFileName, const char* TEMP_DIR, const char* FileName, uint64_t lnum, int nprocs, int rank, uint64_t DRAM_ALLOC)
{
  uint64_t PER_BUF_ALLOC = (DRAM_ALLOC/sizeof(TYPE)/2)*sizeof(TYPE);
  // - Preparation
  // the total size of data in bytes
  uint64_t lsize = lnum * sizeof(TYPE);
  // data size per rank
  uint64_t size_per_rank = lsize/nprocs;
  // number of elements per rank
  uint64_t num_per_rank = lnum/nprocs;
  // number of elements per DRAM
  uint64_t num_per_ram = PER_BUF_ALLOC/sizeof(TYPE);
  if (num_per_ram > num_per_rank)
    num_per_ram = num_per_rank;
#ifdef DEBUG
  fprintf(stderr, "Rank %d: allocating buffers\n", rank);
#endif 
  // allocate ram space 
  TYPE* buf = (TYPE *)malloc((num_per_ram) * sizeof(TYPE));
  int mpi_err;
  // seed rand generator, different for each rank
  srand(rank);

#ifdef DEBUG
  fprintf(stderr, "Rank %d: opening input files\n", rank);
#endif 
  // - Prepare files
  MPI_File in_fh;
  int in_fd;
  MPI_Info info;
  MPI_Status status;
  MPI_Offset offset, LENGTH, file_size;

  // - Binning boundaries
  uint64_t k, start, end, wr_head, wr_start;
  start = rank * num_per_rank;
  end = start + num_per_rank;
  end = (end > lnum)?lnum : end;

#ifdef DEBUG
  fprintf(stderr, "Rank %d: opening input files\n", rank);
#endif 

  // synchroneous open the output file
  if (SortFileName != NULL)
  {
#ifdef DEBUG
  fprintf(stderr, "Rank %d: opening unified input files\n", rank);
#endif 
    MPI_Info_create(&info);
    MPI_File_open( MPI_COMM_WORLD, (char*) SortFileName, MPI_MODE_RDONLY, info, &in_fh );
    wr_start = wr_head = start * sizeof(TYPE);
  } else {
    char filename[2000];
    snprintf(filename, 2000, "%s/%s%d", TEMP_DIR, BINFILE_PREFIX, rank);
#ifdef DEBUG
    fprintf(stderr, "Rank %d: opening individual input files: %s\n", rank, filename);
#endif 
    in_fd = safeOpen(filename, O_RDONLY);
    if (in_fd < 0){
      fprintf(stderr, "Rank %d cannot open %s, giving up binning verification\n", rank, filename);
      free(buf);
      return;
    }
#ifdef DEBUG
  fprintf(stderr, "Rank %d: opened %d\n", rank, filename, in_fd);
#endif 
    // truncate if file existed
    wr_start = wr_head = 0;
    end -= start;
    start = 0;
  }
  
#ifdef DEBUG
  fprintf(stderr, "Rank %d: start verify binning\n", rank);
#endif 

  MPI_Barrier( MPI_COMM_WORLD );

  TYPE lower = rank * num_per_rank;
  TYPE upper = lower + num_per_rank;
  int good_bin = 1;
  for (k = start; k < end; k += num_per_ram)
  {
      // load values to buf
      uint64_t num_per_stage = end - k;
      if (num_per_stage > num_per_ram)
          num_per_stage = num_per_ram;

#ifdef DEBUG
      // write d to the corresponding chunk
      // MPI_Barrier( MPI_COMM_WORLD );
      fprintf(stderr, "Rank %d: reading numbers from %llu to %llu\n", rank, k, k+num_per_stage);
#endif

      // each block is of the same size, we know the offset from the rank
      offset = k * sizeof(TYPE);
      if (SortFileName != NULL)
        MPI_File_read_at( in_fh, offset, (void *) buf, num_per_stage * sizeof(TYPE), MPI_CHAR, &status);
      else
        safeRead(in_fd, buf, num_per_stage*sizeof(TYPE), "verify_read");

#ifdef DEBUG
      fprintf(stderr, "Rank %d: verifying the buffer locally between %llu and %llu\n", rank, lower, upper);
#endif
      // bin elements in buf locally, produce send_cnt[i] = elements fall into the i-th rank's bin
      // localBin(buf, num_per_stage, num_per_rank, nprocs);
      for(int n = 0; n < num_per_stage; n++)
      {
        if(buf[n] < lower || buf[n] >= upper)
        {
          good_bin = 0;
          break;
        }
      }
      if(!good_bin)
        break;
  }
  // - Finishing up
  MPI_Barrier( MPI_COMM_WORLD );
#ifdef DEBUG
  fprintf(stderr, "Rank %d: done\n", rank);
#endif
  if (SortFileName != NULL)
    MPI_File_close(&in_fh);
  else
    close(in_fd);
  free(buf);
  printf("Rank %d: good bin = %d\n", rank, good_bin);
}

