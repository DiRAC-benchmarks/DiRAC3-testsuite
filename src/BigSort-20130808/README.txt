== Usage ==

---- Overall workflow ----

1. Preparing the input:
GenSeq/: generate an ascending sequence of contiguous integers from 0 to NUM(e.g. 134217728)
qsub -n 128 --mode c1 -t 60 -A Performance genseq 134217728 16384L /gpfs/vesta-fs0/projects/Performance/jmeng/data/134217728_unsorted.dat

ShuffleSeq/: shuffle the sequence of numbers and make it a random sequence
/usr/bin/qsub.py -n 128 --mode c1 -t 60 -A Performance shuffle 134217728 4194304L /gpfs/vesta-fs0/projects/Performance/jmeng/data/134217728_unsorted.dat 16 64

2. Sorting:
BigSort/: sort the random sequence (not --env VPROF_PROFILE=yes is for
profiling and is optional)
qsub -n 128 --mode c1 -t 120 -A Performance --env VPROF_PROFILE=yes bigsort 134217728 2097152L /gpfs/vesta-fs0/projects/Performance/jmeng/data/134217728_unsorted.dat /gpfs/vesta-fs0/projects/Performance/jmeng/sorted.134217728.dat /gpfs/vesta-fs0/projects/Performance/jmeng/temp 8388608

3. Validating:
CheckOrder/:
qsub -n 128 --mode c1 -t 30 -A Performance chkorder 134217728 4194304L /gpfs/vesta-fs0/projects/Performance/jmeng/sorted.134217728.dat

---- Individual operations ----
GenSeq: 
Generating an ordered sequence of data from 0 to <num>, stored in <file_name>.
<DRAM_ALLOC> is the amount of DRAM to be used for each rank.
<nodes> is the number of nodes to collectively generating the sequence
qsub -n <nodes> --mode c1 binary <num> <DRAM_ALLOC> <file_name>

CheckOrder:
Check whether a file sequence is in ascending order or not.
<num> is the total number of elements in the file
<file_name> is the name of the file
<DRAM_ALLOC> is the amount of DRAM to be used for each rank.
<nodes> is the number of nodes to collectively generating the sequence
qsub -n <nodes> --mode c1 binary <num> <DRAM_ALLOC> <file_name>

ShuffleSeq:
Shuffle the sequence of numbers stored in a disk file. This produces the input
for sorting.
This is done by repeatedly bringing in scattered "blocks" of data from random locations
in the data file, shuffle them in the DRAM, and then write back to the disk. 
Each round is named as a "stage".
<num> is the total number of elements in the file
<file_name> is the name of the file (used for both input and output)
<DRAM_ALLOC> is the amount of DRAM to be used for each rank.
<nodes> is the number of nodes to collectively generating the sequence
<blks_per_rank> is the number of random blocks of data to shuffle in a stage
<stages> is the number of stages
qsub -n <nodes> --mode c1 binary <num> <DRAM_ALLOC> <file_name> <blks_per_rank> <stages>

CheckBin:
Check whether a file sequence is binned correctly. Only used for validating
the first phase of BigSort
<num> is the total number of elements in the file
<file_name> is the name of the file
<DRAM_ALLOC> is the amount of DRAM to be used for each rank.
<nodes> is the number of nodes to collectively generating the sequence
qsub -n <nodes> --mode c1 binary <num> <DRAM_ALLOC> <file_name>

BigSort:
Sort a big amount of elements that is even larger than the total DRAM for all
nodes. See the introduction section
<num> is the total number of elements in the file
<DRAM_ALLOC> is the amount of DRAM to be used for each rank.
<input_file> is the file containing the input data
<output_file> is the resulting sorted file
<nodes> is the number of nodes to collectively generating the sequence
<temp_dir> is the directory to store temporary, intermediate files
<page_size> is the buffer size for file IO transfers 
qsub -n <nodes> --mode c1 binary <num> <DRAM_ALLOC> <input_file> <output_file> <temp_dir>

== Compile ==
For all the above kernels, the following applies:
* To compile, just "make". The default system is BG/Q. 
* To change the system, see commons/Makefile and pick the right system (e.g.
*   Mira), and do "make CLUSTER=Mira"
* To clean, just "make clean"


== Introduction ==
Sorting a large array of integer values
1. values uniformly fall within a range, denoted by a lower bound and an upper bound.
2. the total data set size is larger than the accumulated memory of all available nodes


== 2-Phase Algorithm ==

1. Bin elements to N chunks, where N = # of nodes. In the end, each chunk serves as a bin for elements falling within a sub range
 a. Read from shared file system.
 b. Exchange elements (MPI communication, AllToAll).
 c. Each rank write to its own local disk copy. Each rank writes a separate "bin file".

2. Sort each "bin file" individually for each node.
 Each node operates over its bin file on its own local disk (No MPI communications)
 Treat the bin file as a collection of chunks, each chunk is of page_size.
 a. sort each chunk
 b. merge every M chunks, repeat until there is only 1 chunk left. 
    Note: M = DRAM_Alloc/(page_size)
 *** A more detailed description ***
 initially, num_runs = num_pages // num_runs is the total number of chunks
 while (num_runs>1)
   // start a new stage to merge
   num_ways = num_pages_in_dram // number of pages to merge at once
   num_merges: num_runs/num_ways: number of merges for all runs

   for each merge:
     (If this is the initial stage, read PAGE_SIZE data from the bin file to create a chunk and sort it. other wise, each run has (PAGE_SIZE*IO_BUF_PAGES) data)
     merge num_runs_in_merge runs // num_runs_in_merge: same as num_ways, handling remainder case
	 each run correspond to a input file, just read the first
 PAGE_SIZE*IO_BUF_PAGES data from the input
	 have and output buffer of PAGE_SIZE*IO_BUF_PAGES, write it to disk
 whenever it is filed
     in the end, each merged array produces a new output file

== Python Code (don't worry, not the actual code, just for fun) ==

def binData1(array, N, chunk_size):
  # exchange individual elements
  chunks = []
  forall n in range(N):
    chunk = array(chunk_size)
    asyncRecvBin(chunk)
    begin = n * chunk_size
    for i in range(begin:begin+chunk_size):
      elem = array[i]
      bin = hashBin(elem)
      # here we send each element individually
      asyncSendBin(elem, bin) 
    barrier()
    chunks.append(chunk)
  return chunks

def binData2(array, N, chunk_size):
  # exchange chunked elements
  chunks = []
  forall n in range(N):
    chunk = array(chunk_size)
    local_bins = array(chunk_size)
    asyncRecvBin(chunk)
    begin = n * chunk_size
    # here we aggregate elements into bins and send them all at once
    local_bins = localBin(array[begin:begin+chunk_size])
    for i in range(N):
      asyncSendBin(local_bins[i], i)
    barrier()
    chunks.append(chunk)
  return chunks

def sortBin(chunk, chunk_size):
  return sort(chunk)
    
T = 10e10
N = 100
chunk_size = T/N
data = array(T)
data = readFromFile('input.dat')
chunks = binData(data, N, chunk_size)
for pos, chunk in enumerate(chunks):
  sorted = sortBin(chunk)
  writeToFile(sorted, pos*chunk_size, 'output.data')



