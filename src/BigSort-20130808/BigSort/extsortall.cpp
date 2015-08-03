
#include "extsort.h"

void extSortAll(char* SortFileName, const char* TEMP_DIR, uint64_t lnum, int nprocs, int rank, uint64_t DRAM_ALLOC, uint64_t page_size)
{
    double tm1, tm2, tm3, tm4;
    // the total size of data in bytes
    uint64_t lsize = lnum * sizeof(TYPE);
    // data size per rank
    uint64_t size_per_rank = lsize/nprocs;
    // TODO: this assumes lnum is dividable by nprocs
    uint64_t num_per_rank = lnum/nprocs;

    // sort the input local file
    char filename[2000];
    snprintf(filename, 2000, "%s/%s%d", TEMP_DIR, BINFILE_PREFIX, rank);

#ifdef DEBUG
  fprintf(stderr, "Rank %d: sorting local file %s\n", rank, filename);
#endif 

    PROF_REPORT_MEM("Before ExtSortFile", rank);
    tm1 = MPI_Wtime();
    // TODO: make sure the sort is in-place, remove intermediate files
    extSortFile(filename, TEMP_DIR, num_per_rank, rank, DRAM_ALLOC, page_size);
    tm2 = MPI_Wtime();
    PROF_REPORT_MEM("After ExtSortFile", rank);

#ifdef DEBUG
  fprintf(stderr, "Rank %d: merging to global file\n", rank);
#endif 

    tm3 = MPI_Wtime();
    // - Prepare files
    MPI_File in_fh, out_fh;
    MPI_Info info;
    MPI_Status status;
    MPI_Offset offset;

    TYPE* buf = (TYPE *)malloc(size_per_rank);
    MPI_Info_create(&info);
    MPI_File_open( MPI_COMM_WORLD, (char*) filename, MPI_MODE_RDONLY, info, &in_fh );
    MPI_File_read_at(in_fh, 0, (void*) buf, size_per_rank, MPI_CHAR, &status);
    MPI_File_close(&in_fh);

    // open the global output file
    MPI_Info_create(&info);
    MPI_File_open( MPI_COMM_WORLD, SortFileName, MPI_MODE_WRONLY|MPI_MODE_CREATE, info, &out_fh );
    // truncate if file existed
    MPI_File_set_size( out_fh, (MPI_Offset) lsize);

    offset = size_per_rank * rank;
    MPI_File_write_at( out_fh, offset, (void*) buf, size_per_rank, MPI_CHAR, &status);

    MPI_File_close(&out_fh);
    free(buf);
    tm4 = MPI_Wtime();

    PROF_REPORT_MEM("After merging ExtSort", rank);
    if ( !rank )
      printf( "ExtSortLocal Time(s) = %f\nMergeGlobal Time(s) = %f\n", tm2 - tm1, tm4 - tm3);

#ifdef DEBUG
  fprintf(stderr, "Rank %d: all done\n", rank);
#endif 
}

