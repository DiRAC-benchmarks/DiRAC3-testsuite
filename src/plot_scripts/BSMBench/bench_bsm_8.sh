#!/bin/bash

#BSUB -n 8
#BSUB -x
#BSUB -R "span[ptile=16]"
#BSUB -J bsm_bench
#BSUB -oo qphix_bench.out.%J
#BSUB -eo qphix_bench.err.%J
#BSUB -q cosma5
#BSUB -P dr002
#BSUB -W 02:00


mpirun -np 8 /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/BSMBench/bsmbench_balance -i /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/BSMBench/sets/balance-8.bsmbench -o /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/BSMBench/output/balance8.out
