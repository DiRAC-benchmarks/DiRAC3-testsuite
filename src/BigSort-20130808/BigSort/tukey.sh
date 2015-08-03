#!/bin/sh

NODES=`cat $COBALT_NODEFILE | wc -l`
#PROCS=$((NODES * 16))
PROCS=$((NODES * 1))
mpirun -f $COBALT_NODEFILE -n $PROCS ./bigsort 4294967296 268435456 /gpfs/mira-fs0/projects/Performance/jmeng/data/4294967296_unsorted_little_endian.dat /gpfs/mira-fs0/projects/Performance/jmeng/test/sorted.data /gpfs/mira-fs0/projects/Performance/jmeng/test/temp 4194304
