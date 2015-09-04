#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
//#include <mpi.h>

extern void Step10_orig( int count1, float xxi, float yyi, float zzi, float fsrrmax2, float mp_rsm2, float *xx1, float *yy1, float *zz1, float *mass1, float *dxi, float *dyi, float *dzi );

#ifdef TIMEBASE
extern unsigned long long timebase();
#else
extern double mysecond();
#endif

// Frequency in MHz, L1 cache size in bytes, Peak MFlops per node */
// BG/Q 
#define MHz 1600e6
#define NC 16777216
#define PEAK (16.*12800.)

// 4 core Intel(R) Xeon(R) CPU E5-2643 0 @ 3.30GHz - Rick's machine celero 
// Peak flop rate: AVX: 8 Flops/cycle * 4 cores * 3291.838 Mcycles/second 
/*
#define MHz 3291.838e6     
#define NC (64*1024*1024)
#define PEAK 105338.816
*/

// 4 core Intel(R) Xeon(R) CPU           E5430  @ 2.66GHz - crush
// Peak flop rate: SSE: 4 Flops/cycle * 4 cores * 2666.666 Mcycles/second
/*
#define MHz 2666.7e6
#define NC ( 256*1024*1024 )
#define PEAK 42667.2
*/

// 4 core Intel(R) Core(TM) i7-3820 CPU @ 3.60GHz - strength
// Peak flop rate: AVX: 8 Flops/cycle * 4 cores * 3600 MCycles/second
/*
#define MHz 3600.e6
#define NC ( 32*1024*1024 )
#define PEAK 115200.
*/

#define N 15000      /* Vector length, must be divisible by 4  15000 */

#define ETOL 1.e-4  /* Tolerance for correctness */

int main( int argc, char *argv[] )
{

  //sid-> float arrays
  static float xx[N], yy[N], zz[N], mass[N], vx1[N], vy1[N], vz1[N];
  //sid-> some more floats
  float fsrrmax2, mp_rsm2, fcoeff, dx1, dy1, dz1;

  //sid-> char arrays
  static char  M1[NC], M2[NC];
  //sid-> some more ints
  int n, count, i, rank, nprocs;

  //sid-> big ints
  unsigned long long tm1, tm2, tm3, tm4, total = 0;

  //sid-> a few doubles
  double t1,t2,t3, elapsed = 0.0, validation, final;

  //sid-> No MPI for ya!

  //MPI_Init( &argc, &argv );
  //MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  //MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
  
  //sid-> but we use the same rank analogy
  rank = 0;
  //sid-> yes sir, this is openmp version
  nprocs = 1;


  //sid-> TODO, mystery number 327, what does this mean?
  count = 327;

  //sid-> executed by the main thread (the only thread rather)
  if ( rank == 0 ) 
  {  
     printf( "count is set %d\n", count );
     printf( "Total MPI ranks %d\n", nprocs );
  } 

  //sid-> the parallel game begins

#pragma omp parallel
{
	//sid-> print number of threads, to be done by the master thread
  if ( (rank == 0) && (omp_get_thread_num() == 0) )
  {

     printf( "Number of OMP threads %d\n\n", omp_get_num_threads() );
     //printf( "      N         Time,us        Validation result\n" );
  }   
}


#ifdef TIMEBASE
  tm3 = timebase();
#endif

  final = 0.;
  //sid-> this looks to be the main loop, TODO, why do we jump 20 and start at 400 boss?
  for ( n = 400; n < N; n = n + 20 ) 
  {
      /* Initial data preparation */
	  //sid-> some physics numbers that I dont care about
	  fcoeff = 0.23f;
      fsrrmax2 = 0.5f; 
      mp_rsm2 = 0.03f;


      //sid-> this is how we do differentiation on the computer, dx/dt - basically divide it as small as possible and add
      dx1 = 1.0f/(float)n;
      dy1 = 2.0f/(float)n;
      dz1 = 3.0f/(float)n;

      //sid-> init the array
      xx[0] = 0.f;
      yy[0] = 0.f;
      zz[0] = 0.f;
      mass[0] = 2.f;
      
      //sid-> init the array - from i=1 to n, sigma(x[i-1]+dx1)
      //sid-> these represent particles
      for ( i = 1; i < n; i++ )
      {
          xx[i] = xx[i-1] + dx1;
          yy[i] = yy[i-1] + dy1;
          zz[i] = zz[i-1] + dz1;
          mass[i] = (float)i * 0.01f + xx[i];
      }

      //sid-> vx is init to zero
    
      for ( i = 0; i < n; i++ )
      {
          vx1[i] = 0.f;
          vy1[i] = 0.f;
          vz1[i] = 0.f;
      }
    
      /* Data preparation done */
    

      //sid-> this is very interesting, clean the L1 cache, flush it with some number, in this case 4. but the point is, will it really do it? TODO
      //sid-> this is the declaration, FYI static char  M1[NC], M2[NC];
      //sid-> it is very interesting to note the importance of static vars in openmp
      //sid-> for this to really clean the cache, NC has to really big, and NC is init to be 16777216
      //sid-> that translates to 16MB or MiB to keep google happy

    
      /* Clean L1 cache */
      for ( i = 0; i < NC; i++ ) M1[i] = 4;
      for ( i = 0; i < NC; i++ ) M2[i] = M1[i];

      //sid-> init some more time variables

#ifdef TIMEBASE
      tm1 = timebase();
#else
      t1 = mysecond();
#endif

      //sid-> this is a nesting, to be executed by each thread
      /*
       * regular stride one memory accesses
       *  It is looping over a predefined list of particles and their neighborhoods and calculates the force values for each particle.
       *
       *
       * The force evaluation for each particle can be independently evaluated
       * the current implementation is using OpenMP “parallel for” pragma statement to parallelize over the loop of particles.
       *
       * The calculation of the force for each particle is implemented as a separate function with particle coordinates, their mass, the
       * list of coordinates of neighborhood particles, and their masses, as the input arguments.
       * */


    /*
     * dx1, dx2 and dz1 are to be private as the values are changed on each iteration
     * */

    #pragma omp parallel for private( dx1, dy1, dz1 )
      for ( i = 0; i < count; ++i)
      {
    	  //sid-> this is the function executed by each thread multiple times on each iteration
        Step10_orig( n, xx[i], yy[i], zz[i], fsrrmax2, mp_rsm2, xx, yy, zz, mass, &dx1, &dy1, &dz1 );
    
        vx1[i] = vx1[i] + dx1 * fcoeff;
        vy1[i] = vy1[i] + dy1 * fcoeff;
        vz1[i] = vz1[i] + dz1 * fcoeff;
      }

      //sid-> some more timing numbers
#ifdef TIMEBASE
      tm2 = timebase();
#else
      t2 = mysecond();
#endif

      //sid-> the threads have finished computing - now validate the results

      validation = 0.;
      for ( i = 0; i < n; i++ )
      {
         validation = validation + ( vx1[i] + vy1[i] + vz1[i] );
      }

      final = final + validation;

      //sid-> what is the time taken in total
    
#ifdef TIMEBASE      
      t3 = 1e6 * (double)(tm2 - tm1) / MHz; // time in us
#else
      t3 = (t2 - t1) * 1e6;
#endif 

      elapsed = elapsed + t3;
    
  }
  //sid-> Print the timing results obtained
#ifdef TIMEBASE
  tm4 = timebase();
  if ( rank == 0 )
  {
      printf( "\nKernel elapsed time, s: %18.8lf\n", elapsed*1e-6 );
      printf(   "Total  elapsed time, s: %18.8lf\n", (double)(tm4 - tm3) / MHz ); 
      printf(   "Result validation: %18.8lf\n", final );
      printf(   "Result expected  : 6636045675.12190628\n" );
  }
#else
  if ( rank == 0 )
  {
      printf( "\nKernel elapsed time, s: %18.8lf\n", elapsed*1e-6 );
  }    
#endif
  
  //MPI_Finalize();

  return 0;
}

