#ifndef __GLOBALS__
#define __GLOBALS__

#include "mpi.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG
#define TYPE uint64_t
bool checkOrder(char* FileName, uint64_t lnum, uint64_t DRAM_ALLOC, int nprocs, int rank);

#endif
