#ifndef EXTSORT_H
#define EXTSORT_H

#include "globals.h"
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <unistd.h>
//#include <fcntl.h>
//#include <pthread.h>
//#include <sys/time.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#ifdef DEBUG
	// #define PRINTF(...) printf(__VA_ARGS__)
	#define PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
	#define PRINTF(...)
#endif


/*
typedef struct {
    int items[PAGE_SIZE];
} page_t;
*/

typedef struct {
    uint64_t length;
    TYPE* items;
} run_t;

void error(const char* msg);
void verify(const char* filename, uint64_t page_size);
void multimerge(run_t** runs, char* input_prefix, int num_runs, int output_fd, int base, uint64_t page_size, int rank);
void extSortFile(const char* DATA_FILENAME, const char* TEMP_DIR, uint64_t lnum, int rank, uint64_t BUF_SIZE, uint64_t page_size);
#endif
