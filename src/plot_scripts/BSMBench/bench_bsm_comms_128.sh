#!/bin/bash

#BSUB -n 128
#BSUB -x
#BSUB -R "span[ptile=16]"
#BSUB -J bsm_bench
#BSUB -oo qphix_bench.out.%J
#BSUB -eo qphix_bench.err.%J
#BSUB -q cosma5
#BSUB -P dr002
#BSUB -W 02:00


mpirun -np 128 /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/BSMBench/bsmbench_comms -i /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/BSMBench/sets/comms-128.bsmbench -o /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/BSMBench/output/comms128.out
