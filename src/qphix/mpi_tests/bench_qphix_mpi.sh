#!/bin/bash

# #BSUB -n 16
# #BSUB -x
# #BSUB -R "span[ptile=1]"
# #BSUB -J qphix_mpi
# #BSUB -oo qphix_bench.out.%J
# #BSUB -eo qphix_bench.err.%J
# #BSUB -q cosma5
# #BSUB -P dr002
# #BSUB -W 10:00


numprocs=16
x=1
for localvol in  "4.4.4.4" "4.4.4.8" "4.4.8.8" "4.8.8.8" "8.8.8.8" "8.8.8.16" "8.8.16.16" "8.16.16.16" "16.16.16.16" "16.16.16.32" "16.16.32.32" "16.32.32.32" "32.32.32.32" 
do
  for block in "-by 8 -bz 8"
  do
    for minct in 2
    do
      for c in 16
      do
	
	 numprocs=16
         declare -a geom=(4 4 4 4)
	 declare -a vol=()
	 vol[0]=0
	 vol[1]=0
	 vol[2]=0
	 vol[3]=0
	 count=0
	 while IFS='.' read -ra ADDR; do
	 	for i in "${ADDR[@]}"; do
			tmp=$(expr $i \* ${geom[count]})
			if [ $tmp -lt 17 ]; then
              			 tmp=$(expr $tmp \* 2)
            		fi
			vol[count]=$tmp
			count=`expr $count + 1`
 		done
	 done <<< "$localvol"

         if [ ${vol[0]} -lt 33 ]; then
             geom[0]=2
	     numprocs=128
         fi

	 global_vol=" -x ${vol[0]} -y ${vol[1]} -z ${vol[2]} -t ${vol[3]}"
	 
         echo "Global Volume ${global_vol}"
	 echo "local volume $localvol"
	 echo "geometry ${geom[@]}"
         export KMP_AFFINITY=compact
#	 echo "/cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/time_dslash_noqdp ${global_vol} $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f -geom ${geom[@]}" > /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/mpi_tests/res/${x}_mpi.txt
 #     	 mpirun -n ${numprocs} -ppn 1 /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/time_dslash_noqdp ${global_vol} $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f -geom ${geom[@]} >> /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi_intel/mpi_tests/res/${x}_mpi.txt
	 x=`expr $x + 1`
      done
    done
  done
done
