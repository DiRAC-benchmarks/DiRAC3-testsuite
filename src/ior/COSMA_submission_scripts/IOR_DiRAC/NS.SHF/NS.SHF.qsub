#!/bin/bash

#BSUB -n 1
#BSUB -x
#BSUB -R "span[ptile=1]"
#BSUB -J  ior_1
#BSUB -oo qphix_bench.out.%J
#BSUB -eo qphix_bench.err.%J
#BSUB -q cosma5
#BSUB -P dr002
#BSUB -W 05:00


DiRAC_TS="/cosma/home/dr002/dc-kash1/cosma5/DiRAC3-testsuite/"

source ${DiRAC_TS}/scripts/cosma_modules.sh
module remove platform_mpi 
module load intel_mpi


DiRAC_Output="${DiRAC_TS}/results"
ID="NS.SHF"

TARGET="${DiRAC_Output}/IOR_DiRAC/${ID}"
mkdir -p $TARGET
mkdir -p ${DiRAC_Output}/ior/${ID}

IOR="${DiRAC_TS}/bin/ior"

IOR_SCRIPT="${DiRAC_TS}/templates/ior/${ID}/con.ior"

echo ${IOR_SCRIPT}

date '+%Y%m%d%H%M%S'

pushd $TARGET

numNodes=1
blockSize=1

echo "mpirun -n ${numNodes} $IOR -vvv  -b ${blockSize}g -f $IOR_SCRIPT"

mpirun -n ${numNodes} $IOR -vvv  -b ${blockSize}g -f $IOR_SCRIPT
