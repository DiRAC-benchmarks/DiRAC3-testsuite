#!/bin/bash

#BSUB -n 256
#BSUB -x
#BSUB -R "span[ptile=16]"
#BSUB -J qphix_mpi
#BSUB -oo qphix_bench.out.%J
#BSUB -eo qphix_bench.err.%J
#BSUB -q cosma5
#BSUB -P dr002
#BSUB -W 15:00

x=1
for localvol in  "4.4.4.4" "4.4.4.8" "4.4.8.8" "4.8.8.8" "8.8.8.8" "8.8.8.16" "8.8.16.16" "8.16.16.16" "16.16.16.16" "16.16.16.32" "16.16.32.32" "16.32.32.32" "32.32.32.32" 
do
  for block in "-by 8 -bz 8"
  do
    for minct in 2
    do
      for c in 16
      do
         declare -a geom=(4 4 4 4)
	 declare -a vol=()
	 count=0
	 while IFS='.' read -ra ADDR; do
	 	for i in "${ADDR[@]}"; do
			tmp=$(expr $i \* ${geom[count]})
			vol=("${vol[@]}" $tmp)
			#echo "tmp $tmp i $i geom ${geom[count]} vol ${vol[@]}"
			count=`expr $count + 1`
 		done
	 done <<< "$localvol"
	 global_vol="-x ${vol[0]} -y ${vol[1]} -z ${vol[2]} -t ${vol[3]}"
	 
         echo "Global Volume ${global_vol}"
	 echo "local volume $localvol"
	 echo "geometry ${geom[@]}"
         export KMP_AFFINITY=compact
	 echo "/cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/time_dslash_noqdp ${global_vol} $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f -geom ${geom[@]}" > /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/mpi_tests/res/${x}_mpi.txt
      	 mpirun -np 256 /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/time_dslash_noqdp ${vol[@]} $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f -geom ${geom[@]} >> /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/mpi_tests/res/${x}_mpi.txt
	 x=`expr $x + 1`
      done
    done
  done
done
