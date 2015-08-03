qsub -t 60 -A Performance -n 16 --env OMP_NUM_THREADS=32 ./tukey.sh
