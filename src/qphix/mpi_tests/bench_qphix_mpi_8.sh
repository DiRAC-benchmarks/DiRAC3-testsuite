#!/bin/bash

#BSUB -n 16
#BSUB -x
#BSUB -R "span[ptile=1]"
#BSUB -J qphix_mpi
#BSUB -oo qphix_bench.out.%J
#BSUB -eo qphix_bench.err.%J
#BSUB -q cosma5
#BSUB -P dr002
#BSUB -W 05:00
module remove platform_mpi
module load intel_mpi
numprocs=16





numprocs=16
x=1
for localvol in  "16.16.16.32"  
do
  for block in "-by 8 -bz 8"
  do
    for minct in 2
    do
      for c in 16
      do
	
	 



numprocs=16
         declare -a geom=(2 2 2 2)
	 declare -a vol=()
	 vol[0]=0
	 vol[1]=0
	 vol[2]=0
	 vol[3]=0
	 count=0
	 while IFS='.' read -ra ADDR; do
	 	for i in "${ADDR[@]}"; do
			tmp=$(expr $i \* ${geom[count]})
			vol[count]=$tmp
			count=`expr $count + 1`
 		done
	 done <<< "$localvol"

         if [ ${vol[0]} -lt 33 ]; then
             vol[0]=32
         fi


	 global_vol=" -x ${vol[0]} -y ${vol[1]} -z ${vol[2]} -t ${vol[3]}"
	 
         echo "Global Volume ${global_vol}"
	 echo "local volume $localvol"
	 echo "geometry ${geom[@]}"
         export KMP_AFFINITY=compact
	 echo "/cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/time_dslash_noqdp ${global_vol} $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f -geom ${geom[@]}" > /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/mpi_tests/res/${x}_mpi_${localvol}.txt
     	 mpirun -n ${numprocs} -ppn 1 /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/time_dslash_noqdp ${global_vol} $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f -geom ${geom[@]} >> /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/mpi_tests/res/${x}_mpi_${localvol}.txt
	 x=`expr $x + 1`
      done
    done
  done
done
