
# BSMBench -- A HPC Benchmark for BSM Lattice Physics  
### Version 1.0


Contents
========

1. Compiling BSMBench
    1.1 Machine configuration files
    1.2 Compilation script
2. Running BSMBench
3. Interpreting output
4. Version history


# 1. Compiling BSMBench
###1.1. Machine configuration file

This file specifies your C compiler and its flags. The syntax is
  
    CC = [C compiler]
    CFLAGS = [flags]

Some examples are given in the machine-config directory. The files BGP.cfg
and BGP-1.cfg contain the parameters used for the Blue Gene/P performance
comparison data, which have been obtained running the code with the
environment variable DCMF_INTERRUPT set to 1.

###1.2 Compilation script

By default, the compilation script compiles only MPI versions of the 
benchmark. In this case it takes a single parameter: the machine 
configuration file. For example

    ./make.sh machine-config/generic.cfg

will compile a version suitable for a generic cluster.

If a second parameter is included, it is used as the machine configuration
file for a non-MPI version. In this case, two sets of executables are
generated, one ending in _1cpu. For example

    ./make.sh machine-config/BGP.cfg machine-config/BGP-1.cfg

will generate MPI and non-MPI files for a Blue Gene/P machine.

To avoid generating the MPI versions, set the NO_MPI environmental variable
and pass a dummy first parameter. For example

    export NO_MPI=YES
    ./make.sh xxxx machine-config/OSX-1.cfg

will generate non-MPI executables only for a machine running Mac OS X.

Three files are generated in each set:
    - bsmbench_comms[_1cpu]:    Runs the communications test
    - bsmbench_balance[_1cpu]:  Runs the balanced test
    - bsmbench_compute[_1cpu]:  Runs the compute test

#2. Running BSMBench

Before running a test with BSMBench, you must know:
    - which test you want to run (comms, balance, or compute)
    - how many processes you want to use

BSMBench is single-threaded, so ideally the number of processes should
be equal to the number of cores (for example, Virtual Node mode on Blue
Gene/P), or potentially more if the machine has multithreading cores.

The general syntax for running BSMBench is then:

    ./bsmbench_[test]{_1cpu} -i [input_file] -o [output file]

Input files are located in the sets directory. They are named
systematically as [test]-[number-of-processes].bsmbench. The output
directory is provided for convenience as a location to put output files.

For example, to run the balance test on a single core, the syntax could be

    ./bsmbench_balance_1cpu -i sets/balance-1.bsmbench -o output/bal-1.out

To run the tests in parallel, mpirun must be used. For example, to run the
compute test on 64 cores, the syntax could be

    mpirun -np 64 ./bsmbench_compute -i sets/compute-64.bsmbench
        -o output/compute-64.out

mpirun can then be called by your queueing system of choice.

On Blue Gene machines please export the environmental variable:

     DCMF_INTERRUPT=1


#3. Interpreting output

Output files start with some general remarks about the parameters used,
marked with [MAIN], then move on to the benchmark results, marked with
[BENCH]. To pull out the relevant results, grep can be used:

    grep BENCH [output-file]

First, an indication is given of whether the precision test was successful.
Then, for each performance test, the output consists of:

    - the time taken
    - the average FLOP/s rating for the test
    - the average FLOP/s per process for the test
    - (Dphi only) a relative comparison to the speed of a Blue Gene/P
      machine performing the same test

Following this, an indication of success is given, followed by the average
FLOP/s and FLOP/s per process across all tests, and a relative comparison
to the speed of a Blue Gene/P machine performing the same test.

Of the tests, the Dphi is the most relevant for performing lattice
calculations, and for testing communications bandwidth, so results from 
this test are compared with reference machines separately from the total
aggregate figure. It is recommended that this figure be weighted most
strongly in any comparison.


# 4. Version History

Version 1.0 (2012-04-19)
    - Initial release