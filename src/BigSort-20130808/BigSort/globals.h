#ifndef __GLOBALS__
#define __GLOBALS__

#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#define TYPE uint64_t
#define MIN_TYPE 0LU // The min alue of this type
#define MAX_TYPE 18446744073709551615LU // The max alue of this type

#define VERIFY_ALLOC 67108864

#define BIN_PAGE_SIZE 8192

//#define DEBUG
//#define PROF

int safeOpen(const char *pathname,int flags, mode_t mode);
int safeOpen(const char *pathname,int flags);
ssize_t safeRead(int fd, void *buf, size_t count, const char* marker);
ssize_t safeWrite(int fd, const void *buf, size_t count, const char* marker);
void iotime();

void QuickSort(TYPE* Buf, long long Lower, long long Upper, int rank=-1);
void QuickSortPar(TYPE* Buf, long long Lower, long long Upper, int rank=-1);
void QuickSortInt(int* Buf, long long Lower, long long Upper, int rank=-1);
void bining(const char* FileName, const char* TEMP_DIR, const char* SortFileName, uint64_t lnum, int nprocs, int rank, uint64_t DRAM_ALLOC);
void extSortAll(char* SortFileName, const char* TEMP_DIR, uint64_t lnum, int nprocs, int rank, uint64_t DRAM_ALLOC, uint64_t page_size);
void verifyBins(const char* SortFileName, const char* TEMP_DIR, const char* FileName, uint64_t lnum, int nprocs, int rank, uint64_t DRAM_ALLOC);
bool checkOrder(char* FileName, uint64_t lnum, uint64_t DRAM_ALLOC, int nprocs, int rank);

#define BINFILE_PREFIX "sort.bin."
#define EXTSORT_SWAP_PREFIX1 "yin"
#define EXTSORT_SWAP_PREFIX2 "yang"


#ifdef PROF
  #include "/bgsys/drivers/V1R2M0/ppc64/spi/include/kernel/memory.h"
  #define PROF_REPORT_MEM(string, rank) \
  {\
    uint64_t heap_space; \
    uint64_t stack_space; \
    uint64_t mmap_space; \
    int perr; \
    perr = Kernel_GetMemorySize(KERNEL_MEMSIZE_HEAP, &heap_space);\
    perr = Kernel_GetMemorySize(KERNEL_MEMSIZE_STACK, &stack_space);\
    perr = Kernel_GetMemorySize(KERNEL_MEMSIZE_MMAP, &mmap_space);\
    if (!rank) {\
      fprintf(stderr, "Prof-%s: used heap size in MB: %lf. ret=%d\n", string, double(heap_space)/(1<<20), perr); \
      fprintf(stderr, "Prof-%s: used stack size in MB: %lf. ret=%d\n", string, double(stack_space)/(1<<20), perr); \
      fprintf(stderr, "Prof-%s: used mmap size in MB: %lf. ret=%d\n", string, double(mmap_space)/(1<<20), perr); \
    }\
  }
#else
  #define PROF_REPORT_MEM(string, rank)
#endif

#endif
