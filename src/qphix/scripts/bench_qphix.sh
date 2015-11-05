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




for vol in "-x 4 -y 4 -z 4 -t 4 " "-x 4 -y 4 -z 4 -t 8 " "-x 4 -y 4 -z 8 -t 8 " "-x 4 -y 8 -z 8 -t 8 " "-x 8 -y 8 -z 8 -t 8 " "-x 8 -y 8 -z 8 -t 16 " "-x 8 -y 8 -z 16 -t 16 " "-x 8 -y 16 -z 16 -t 16 " "-x 16 -y 16 -z 16 -t 16 " "-x 16 -y 16 -z 16 -t 32 " "-x 16 -y 16 -z 32 -t 32 " "-x 16 -y 32 -z 32 -t 32 " "-x 32 -y 32 -z 32 -t 32 " "-x 32 -y 32 -z 32 -t 64 " "-x 32 -y 32 -z 64 -t 64 " "-x 32 -y 64 -z 64 -t 64 " "-x 64 -y 64 -z 64 -t 64 "
do
  for block in "-by 4 -bz 4"  "-by 4 -bz 8"  "-by 8 -bz 8"  "-by 8 -bz 16"  "-by 16 -bz 16"
  do
    for minct in 1 2 
    do
      for c in `seq 16`
      do
      var=`echo $vol|sed -e 's/  *$//'`
      echo "./time_dslash_noqdp $vol $block -pxy 1 -pxyz 0 -c $c -sy 1 -sz 2 -minct $minct -compress12 -geom 1 1 1 1 -i 500 --prec=f" > /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/scripts/bench_qphix_${var}_${block}_${c}_${minct}.result
      done
    done
  done
done
