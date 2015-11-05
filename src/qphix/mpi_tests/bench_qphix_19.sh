#!/bin/bash

# Lx is the lattice size in X
# Ly is the lattice size in Y
# Lz is the lattice size in Z
# iters is the number of iterations
# BY is the block size in Y
# BZ is the block size in Z
# NCores is the no of cores
# Sy is the no of simt threads in y
# Sz is the no of simt threads in z
# Pxy is the extra pad in the XY plane
# Pxyz is the extra pad in the XYZ plane
# MinCt is the MinCt parameter in the blocking scheme
# Px Py Pz Pt define a 4D grid of MPI tasks
# Prec for precision

x=1
for vol in "-x 24 -y 24 -z 24 -t 128 "

do
  for block in "-by 4 -bz 4"  "-by 4 -bz 8"  "-by 8 -bz 8"  "-by 8 -bz 16"  "-by 16 -bz 16"
  do
    for minct in 1 2
    do
      for c in `seq 16 -1 1`
      do

        export KMP_AFFINITY=compact
echo "/cosma/home/dr002/dc-kash1/cosma5/DiRAC3-testsuite/src/qphix/build_soa4/tests/time_dslash_noqdp $vol $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f" > /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_soa4/tests/soa4_bench/${x}_12.txt

      /cosma/home/dr002/dc-kash1/cosma5/DiRAC3-testsuite/src/qphix/build_soa4/tests/time_dslash_noqdp $vol $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12  -i 500 -prec f >> /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_soa4/tests/soa4_bench/${x}_12.txt

	x=`expr $x + 1`
      done
    done
  done
done
