#include "extsort.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
// from https://github.com/umbrant/extsort.git

//extern int errno; 

void extSortFile(const char* DATA_FILENAME, const char* TEMP_DIR, uint64_t lnum, int rank, uint64_t BUF_SIZE, uint64_t PAGE_SIZE)
{
    double tm1, tm2, tm3, tm4, tm_quicksort=0., tm_merge=0.;

    uint64_t DATA_SIZE = lnum * sizeof(TYPE);
    uint64_t NUM_PER_PAGE = (PAGE_SIZE/sizeof(TYPE));
    char filename[2000];
    snprintf(filename, 2000, "%s", DATA_FILENAME);

    int sort_fd = safeOpen(filename, O_RDONLY);

#ifdef DEBUG
  fprintf(stderr, "Rank %d: opened file %s\n", rank, filename);
#endif 

    PROF_REPORT_MEM("ExtSort: after Opening sort.bin", rank);

    // initially, each run is just 1 page, so this is how many runs there are
    unsigned long run_length = 1;
    unsigned long num_runs = DATA_SIZE / PAGE_SIZE;
    if(DATA_SIZE % PAGE_SIZE) {
        num_runs++;
    }
    unsigned long total_num_runs = num_runs;

#ifdef DEBUG
    fprintf(stderr, "Sorting %lu ints (%.02f MB)...\n", 
            num_runs*NUM_PER_PAGE,
            ((double)(num_runs*PAGE_SIZE)) / (1<<20));
    fprintf(stderr, "num_runs=%lu\n", num_runs);
#endif


    char input_prefix[2000];
    char output_prefix[2000];
    snprintf(input_prefix, 2000, "%s/%s_%d_", TEMP_DIR, EXTSORT_SWAP_PREFIX1, rank);
    snprintf(output_prefix, 2000, "%s/%s_%d_", TEMP_DIR, EXTSORT_SWAP_PREFIX2, rank);

    bool init = true;
    while(num_runs > 1 || init) {
        init = false;
#ifdef DEBUG
        fprintf(stderr, "Rank %d: Iterate: %lu runs left\n", rank, num_runs);
#endif
        // Number of ways we can merge at once
        unsigned long num_ways = ((BUF_SIZE / PAGE_SIZE)) - 1;
        // Case for small sorts: fits in memory
        if(num_ways > num_runs) {
            num_ways = num_runs;
        }
        // Calculate how many multimerges need to be done to merge all runs
        unsigned long num_merges = num_runs / num_ways;
        if(num_runs % num_ways) {
            num_merges++;
        }

        unsigned long run_counter = 0;

        // Start iterating the multimerges
        for(int i=0; i<num_merges; i++) {
#ifdef DEBUG
            fprintf(stderr, "Rank %d: Merge %d of %lu (%lu ways)\n", rank, i+1, num_merges, num_ways);
#endif
            // Normal case: merge n-ways
            unsigned long num_runs_in_merge = num_ways;
            // Remainder case: merge num_runs % num_ways
            if(run_counter >= (num_runs/num_ways)*num_ways) {
                num_runs_in_merge = num_runs % num_ways;
            }

            // Allocate the runs and runfds we're using for this multimerge
            run_t** runs = (run_t**)calloc(num_runs_in_merge, sizeof(run_t*));

    	    PROF_REPORT_MEM("ExtSort: after allocating runs", rank);
            // Handle the first pass differently, since the numbers
            // are all coming out of the same file, we need to populate
            // the runs here instead of letting multimerge do it
            if(run_length == 1) {
                // Allocate different sized runs
                PRINTF("Rank %d: Base case: init and sorting pages\n", rank);
                for(int i=0; i<num_runs_in_merge; i++) {
                    runs[i] = (run_t*)calloc(1, sizeof(run_t));
                    runs[i]->items = (TYPE*)calloc(1, PAGE_SIZE);
                    uint64_t bytes = safeRead(sort_fd, runs[i]->items, PAGE_SIZE, "first pass");
                    runs[i]->length = bytes/sizeof(TYPE);
                    if(bytes == 0) {
                        break;
                    }
                }
                PROF_REPORT_MEM("ExtSort: after loading sort.bin", rank);
                run_counter += num_runs_in_merge;

                PRINTF("Rank %d: Doing quick sort...\n", rank);
  		tm1 = MPI_Wtime();
                #pragma omp parallel for schedule(dynamic)
                for(int n = 0; n < num_runs_in_merge; n++)
                {
                    if(runs[n]->length > 1){
                        PRINTF("Rank %d: QuickSort %lu, length = %lu\n", rank, runs[n]->items, runs[n]->length);
                        QuickSort(runs[n]->items, 0, runs[n]->length-1, rank);
                    }
                }
  		tm2 = MPI_Wtime();
                tm_quicksort += tm2-tm1;
                PRINTF("Rank %d: Finish quick sort!\n", rank);
                // Verify the quicksort
#ifdef DEBUG
                PRINTF("Rank %d: Verifying qsort results...", rank);
                for(int i=0; i<num_runs_in_merge; i++) {
                    TYPE temp = 0;
                    int init = 0;
                    for(uint64_t j=0; j<runs[i]->length; j++) {
                        if(!init) {
                            temp = runs[i]->items[j];
                            init = 1;
                        }

                        if(runs[i]->items[j] < temp) {
                            printf("Incorrect qsort! Run %d idx %d\n",
                                    i, j);
                            exit(-1);
                        }
                        temp = runs[i]->items[j];
                    }
                }
                PRINTF("Rank %d: correct!\n", rank);
#endif
                PROF_REPORT_MEM("ExtSort: after sorting each page", rank);
            }
            else {
                for(int i=0; i<num_runs_in_merge; i++) {
                    snprintf(filename, 2000, "%s%lu.dat", input_prefix, run_counter);
                    PRINTF("Rank %d: Reading in: %s\n", rank, filename);
                    run_counter++;
                }
            }

            snprintf(filename, 2000, "%s%d.dat", output_prefix, i);
            PRINTF("Rank %d: Writing out: %s\n", rank, filename);
            int output_fd = safeOpen(filename, O_CREAT|O_WRONLY|O_TRUNC, 
                    S_IRWXU|S_IRWXG);
            // Merge them together
            int base = 0;
            if(run_length == 1) {
                base = 1;
            }
            tm3 = MPI_Wtime();
            multimerge(runs, input_prefix, num_runs_in_merge, output_fd, base, PAGE_SIZE, rank);
            tm4 = MPI_Wtime();
            tm_merge += tm4-tm3;

            PROF_REPORT_MEM("ExtSort: multimerge, before close the output file", rank);

            close(output_fd);

            // Close and free runs / fds
            // Free the allocated runs if base case
            if(base == 1) {
                for(int i=0; i<num_runs_in_merge; i++) {
                    free(runs[i]->items);
                    free(runs[i]);
                }
            }
            free(runs);
            PROF_REPORT_MEM("ExtSort: finishing each merge", rank);
        }
        if(run_length == 1 && num_ways != 1) {
            close(sort_fd);
        }
        // Remove the input file
        for(int i=0; i<num_runs; i++) {
            snprintf(filename, 2000, "%s%d.dat", input_prefix, i);
            remove(filename);
        }
        // Swap the input and output prefixes
        // This way the next merge uses the previous round's output
        // as input
        strcpy(filename, output_prefix);
        strcpy(output_prefix, input_prefix);
        strcpy(input_prefix, filename);

        // RUN_LENGTH increases by N_WAYS every iteration
        run_length *= num_ways;
        // Number of runs produced is one per merge, i.e. num_merges
        num_runs = num_merges;
        PROF_REPORT_MEM("ExtSort: after each stage", rank);
    }

    snprintf(filename, 2000, "%s%d.dat", input_prefix, 0);
    rename(filename, DATA_FILENAME);
    if(!rank)
      printf("QuickSort Time(s) = %f\nMergeSort Time(s) = %f\n", tm_quicksort, tm_merge);
#ifdef DEBUG
    verify(DATA_FILENAME, PAGE_SIZE);
#endif
}


/* multimerge
 *
 * On the first pass, runs are passed already allocated. This is
 * indicated by the boolean <base> parameter
 *
 * Further passes need to allocate io_buf_pages for each run and read
 * in based on the input_prefix and num_runs. We can't keep the fds
 * open because there might be too many, so reopen lazily.
 *
 * output_fd comes already open and initialized for writing.
 */
void multimerge(run_t** runs, char* input_prefix, int num_runs, int output_fd, int base, uint64_t PAGE_SIZE, int rank)
{
    // Allocate run bufs if not base case
    uint64_t run_offsets[num_runs];
    uint64_t NUM_PER_PAGE = (PAGE_SIZE/sizeof(TYPE));
    char filename[2000];
    if(base == 0) {
        PRINTF("multimerge: allocating run bufs\n");
        for(int i=0; i<num_runs; i++) {
            runs[i] = (run_t*)calloc(1, sizeof(run_t));
            runs[i]->items = (TYPE*)calloc(1, PAGE_SIZE);
            snprintf(filename, 2000, "%s%d.dat", input_prefix, i);
            int fd = safeOpen(filename, O_RDONLY);
            uint64_t bytes = safeRead(fd, runs[i]->items, PAGE_SIZE, "multimerge first pass");
            close(fd);

            runs[i]->length = bytes/sizeof(TYPE);
            run_offsets[i] = bytes;
        }
    }

    PROF_REPORT_MEM("ExtSort: after allocating multimerge input", rank);

    uint64_t idx[num_runs];
    int skip[num_runs];

    // TODO: this does not count in DRAM_ALLOC
    TYPE* output = (TYPE*)malloc(PAGE_SIZE);
    uint64_t output_idx = 0;

    PROF_REPORT_MEM("ExtSort: after allocating multimerge output", rank);

    // This decrements every time a run is fully merged
    int live_runs = num_runs;

    for(int i=0; i<num_runs; i++) {
        idx[i] = 0;
        // Could conceivably be passed null runs (you are a
        // horrible person if you do this).
        if(runs[i]->length > 0) {
            skip[i] = 0;
        } else {
            skip[i] = 1;
            live_runs--;
        }
    }

    // Merge runs together
    TYPE* wavefront = (TYPE*) malloc(num_runs * sizeof(TYPE));
    // uint64_t* wavefront_runs = (uint64_t*) malloc(num_runs * sizeof(uint64_t));
    for (int j=0; j<num_runs; j++)
    {
      if(runs[j]->length > 0)
        wavefront[j] = runs[j]->items[0];
      // wavefront_runs[j] = j;
    }
    
    while(live_runs > 0) {
        // find min element across runs
        TYPE min = MAX_TYPE;
        int min_run = -1;
        for(int j=0; j<num_runs; j++) {
            if(!skip[j]) {
                if(wavefront[j] < min) {
                    min_run = j;
                    min = wavefront[j];
                }
            }
        }
        // put min into output buffer
        output[output_idx] = min;
        idx[min_run]++;
        if(idx[min_run] < runs[min_run]->length)
          wavefront[min_run] = runs[min_run]->items[idx[min_run]];

        // Read in more of the run if the buf is done
        if(!skip[min_run] && idx[min_run] == runs[min_run]->length) {
            if(base) {
                skip[min_run] = 1;
                live_runs--;
            } else {
                // Reopen min_run's fd at right offset to get next chunk
                snprintf(filename, 2000, "%s%d.dat", input_prefix, min_run);
                int fd = safeOpen(filename, O_RDONLY);
                lseek(fd, run_offsets[min_run], SEEK_SET);
                uint64_t bytes = safeRead(fd, runs[min_run]->items, PAGE_SIZE, "multimerge");
                if(bytes > 0) {
                    idx[min_run] = 0;
                    wavefront[min_run] = runs[min_run]->items[0];
                    runs[min_run]->length = bytes / sizeof(TYPE);
                    // skip the run in the future if the file is completely read
                } else {
                    skip[min_run] = 1;
                    live_runs--;
                }
                close(fd);
                run_offsets[min_run] += bytes;
            }
        }
        // Flush the write buffer to disk if necessary
        output_idx++;
        if(output_idx == NUM_PER_PAGE) {
            // Verify the buf first
            TYPE check = output[0];
            for(uint64_t j=1; j<NUM_PER_PAGE; j++) {
                if(output[j] < check) {
                    fprintf(stderr, "Error at index %lu. check = %lu, got %lu\n", j, check, output[j]);
                }
                check = output[j];
            }
            // TODO: double buffer and use aio calls
            uint64_t rv = safeWrite(output_fd, output, PAGE_SIZE, "dumpmerging");
            if(rv != PAGE_SIZE) {
                fprintf(stderr, "Write size mismatch during merging! expecting %lu, got %lu\n", PAGE_SIZE, rv);
            }
            output_idx = 0;
        }
    }
    PROF_REPORT_MEM("ExtSort: finishing one multimerge, before free all buffers", rank);
    // Do a final flush
    uint64_t bytes = safeWrite(output_fd, output, output_idx*sizeof(TYPE), "finalflush");
    if(bytes != output_idx*sizeof(TYPE)) {
        fprintf(stderr, "Write size mismatch during final flush! expecting %lu, got %lu\n", output_idx*sizeof(TYPE), bytes);
    }

    // Free run bufs if not base case
    if(base == 0) {
        for(int i=0; i<num_runs; i++) {
            free(runs[i]->items);
            free(runs[i]);
        }
    }
    free(wavefront);
    free(output);
}


void verify(const char* filename, uint64_t PAGE_SIZE)
{
    printf("Verifying sort (%s)...\n", filename);
    int input_fd = safeOpen(filename, O_RDONLY);
    // Get the data file's size
    struct stat s;
    int rv = fstat(input_fd, &s);
    if(rv) {
        error("Could not fstat file!");
    }
    uint64_t buf_size = PAGE_SIZE;
    int num_runs = s.st_size / (buf_size);
    if(s.st_size % buf_size) {
        num_runs++;
    }

    TYPE* buf = (TYPE*)calloc(1, buf_size);
    TYPE current = 0;
    int init = 0;
    int error = 0;
    uint64_t count = 0;
    while(num_runs > 0) {
        uint64_t bytes = safeRead(input_fd, buf, buf_size, "verify");
        for(uint64_t i=0; i<bytes/sizeof(TYPE); i++) {
            if(!init) {
                current = buf[i];
                init = 1;
            }
            if(buf[i] < current) {
                error = 1;
            }
            //fprintf(stderr, "%d\n", current);
            current = buf[i];
            count++;
        }
        num_runs--;
    }
    //fprintf(stderr, "%d\n", current);

    if(error) {
        printf("Incorrect sort of %lu ints %s.\n", count, filename);
    } else {
        printf("Correctly sorted! %lu ints %s.\n", count, filename);
    }
    free(buf);
}

// TODO: delete or revise with mpi_finalize
void error(const char* msg)
{
    perror(msg);
    exit(-1);
}

/*
void usage()
{
    printf("extsort <filename> <buffer_size_mb> <io_buf_pages>\n");
    printf("\tfilename: filename of file to be sorted\n");
    printf("\tbuffer_size_mb: MBs of buffer to use for sorting\n");
    printf("\tio_buf_pages: number of 4k pages to use for per-run IO buffering\n");
    exit(1);
}


long get_time_usecs()
{
    struct timeval time;
    struct timezone tz;
    memset(&tz, '\0', sizeof(timezone));
    gettimeofday(&time, &tz);
    long usecs = time.tv_sec*1000000 + time.tv_usec;

    return usecs;
}
    long start_usecs = get_time_usecs();
    long end_usecs = get_time_usecs();
    double secs = (double)(end_usecs - start_usecs) / (double)1000000;
*/

