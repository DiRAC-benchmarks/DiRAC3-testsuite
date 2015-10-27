 /*******************************************************************************\
 * Copyright (c) 2008, Claudio Pica, Antonio Rago                                *   
 * All rights reserved.                                                          *
 *     	      									 *
 * Redistribution and use in source and binary forms, with or without            *
 * modification, are permitted provided that the following conditions are met:   *
 *                                                                               *
 * 1)  Redistributions of source code must retain the above copyright notice,    *
 *     this list of conditions and the following disclaimer.                     *
 * 2)  Redistributions in binary form must reproduce the above copyright notice, *
 *     this list of conditions and the following disclaimer in the documentation *
 *     and/or other materials provided with the distribution.                    *
 * 3)  Any publication in any form derived from the use of this software or any  *
 *     modification of it must refer explicitly to the original BSMBench package *
 *     (including the official URL http://github.com/blucini/BSMBench)           *
 *     and cite the following two publications:                                  *
 *       [1] L. Del Debbio, A. Patella, C. Pica, "Higher representations on the  *
 *           lattice: Numerical simulations. SU(2) with adjoint fermions", Phys. *
 * 	  Rev. D81 (2010) 094503, DOI 10.1103/PhysRevD.81.094503                 *
 *       [2] L. Del Debbio, B. Lucini, A. Patella, C. Pica, A. Rago, "Conformal  *
 *       versus confining scenario in SU(2) with adjoint fermions", Phys.Rev.    *
 *       D80 (2009) 074507, DOI 10.1103/PhysRevD.80.074507                       *
 * 4)  The names of the contributors may be used to endorse or promote products  *
 *     derived from this software without specific prior written permission.     *
 *                                                                               *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"   *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE     *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE    *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE     *
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF          *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS      *
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN       *
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       *
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF        *
 * THE POSSIBILITY OF SUCH DAMAGE.                                               *
 \*******************************************************************************/

 /***********************************************************************************
 *
 * File geometry.c
 *
 * Definition of the lattice geometry
 * 
 * In this file we define a geometry for a part of the total lattice that must be run 
 * by a parallel(mpi) code.
 * In particular we define the geometry of the subset of the lattice that run on a 
 * single node.
 * We have decided to include in this definition also a buffer that will contain 
 * all the neighbours we need for a generic update.
 * This local lattice is then also divided in three mayor pieces:
 *
 * Inner lattice: the part of the local lattice that don't needs any information by other 
 *                processors to be upgraded
 * Border Lattice: the part of the local lattice that needs other information from
 *                 other processors
 * Buffer lattice: is the buffer in which we will put the fields sended by other processor
 *                
 *                
 *                GEOMETRY
 * 1)We call X Y Z T the dimensions of the LOCAL lattice
 *   and [X|Y|Z|T]_BORDER the variable that specify the dimension in each direction of
 *   the border.                
 * 2)If the local lattice is compactified along one direction, then all the informations 
 *   needed along that direction are contained in the local lattice itself, then
 *   the Border of that direction is putted to zero.
 * 3)The definition of the compactified/open boundaries depends on the number of
 *   processors along that peculiar direction.
 * 4)The actual convection is to split the all local lattice in 4 levels
 *   level 4 points that belongs only to inner lattice
 *   level 3 points that belongs to only one border
 *   level 2 points that belongs to two borders
 *   level 1 points that belongs to 3 borders
 * 5)the organization of the string of data is then the following
 *   |-Level 4(inner lattice)-|-(X Brd)-|-(Y Brd)-|-(Z Brd)-|-(T Brd)-|-(X Bffr)-|-(Y Bffr)-|-(Z Bffr)-|-(T Bffr)-|
 *   
 *   |-(X Brd)-| is organized in this way
 *   {(0:X_BORDER),(0:Y_BORDER),(0:Z),(0:T)},{(0:X_BORDER),(0:Y),(0:Z_BORDER),(0:T)},{(0:X_BORDER),(0:Y),(0:Z),(0:T_BORDER)} (this line is Level 2)
 *   {(X_BORDER:X-X_BORDER),(Y_BORDER:Y-Y_BORDER),(Z_BORDER:Z-Z_BORDER),(T_BORDER:T-T_BORDER)} (this line is Level 3)
 *
 *   |-(Y Brd)-| is organized in this way
 *   {(0:X_BORDER),(0:Y_BORDER),(0:Z),(0:T)},{(0:X),(0:Y_BORDER),(0:Z_BORDER),(0:T)},{(0:X),(0:Y_BORDER),(0:Z),(0:T_BORDER)}
 *   {(X_BORDER:X-X_BORDER),(Y_BORDER:Y-Y_BORDER),(Z_BORDER:Z-Z_BORDER),(T_BORDER:T-T_BORDER)}
 *
 *   |-(Z Brd)-| is organized in this way
 *   {(0:X_BORDER),(0:Y),(0:Z_BORDER),(0:T)},{(0:X),(0:Y_BORDER),(0:Z_BORDER),(0:T)},{(0:X),(0:Y),(0:Z_BORDER),(0:T_BORDER)}
 *   {(X_BORDER:X-X_BORDER),(Y_BORDER:Y-Y_BORDER),(Z_BORDER:Z-Z_BORDER),(T_BORDER:T-T_BORDER)}
 *
 *   |-(T Brd)-| is organized in this way
 *   {(0:X_BORDER),(0:Y),(0:Z),(0:T_BORDER)},{(0:X),(0:Y_BORDER),(0:Z),(0:T_BORDER)},{(0:X),(0:Y),(0:Z_BORDER),(0:T_BORDER)}
 *   {(X_BORDER:X-X_BORDER),(Y_BORDER:Y-Y_BORDER),(Z_BORDER:Z-Z_BORDER),(T_BORDER:T-T_BORDER)}
 *
 *   the buffers are organized in the same way of the corresponding borders
 *
 *                SIZE
 * Inner lattice SIZE =(X-2*X_BORDER)*(Y-2*Y_BORDER)*(Z-2*Z_BORDER)*(T-2*T_BORDER)*
 * Border lattice SIZE =
 * 2*X_BORDER*(2*Y_BORDER*Z*T+2*Y*Z_BORDER*T+2*Y*Z*T_BORDER+(Y-2*Y_BORDER)*(Z-2*Z_BORDER)*(T-2*T_BORDER))+
 * 2*Y_BORDER*(2*Z_BORDER*T*X+2*Z*T_BORDER*X+2*Z*T*X_BORDER+(X-2*X_BORDER)*(Z-2*Z_BORDER)*(T-2*T_BORDER))+
 * 2*Z_BORDER*(2*T_BORDER*X*Y+2*T*X_BORDER*Y+2*T*X*Y_BORDER+(Y-2*Y_BORDER)*(X-2*X_BORDER)*(T-2*T_BORDER))+
 * 2*T_BORDER*(2*X_BORDER*Y*Z+2*X*Y_BORDER*Z+2*X*Y*Z_BORDER+(Y-2*Y_BORDER)*(Z-2*Z_BORDER)*(X-2*X_BORDER))
 * Buffer lattice SIZE = Border lattice SIZE
 *
 *
 *       THE CODE
 * The underlying idea is to have an index that runs on a bigger lattice (X+2*X_BORDER)*(Y+2*Y_BORDER)*(Z+2*Z_BORDER)*(T+2*T_BORDER)
 * and a list of steps that cover even with copies the lattices, and then create two maps between these two indices
 * In particular we walk on the lattice in the inner lattice using a pattern that tries to optimize the disposition of the sites
 * for the automatic fetching of the processor.
 * Also for the border (level 3) this is true but we have to avoid the already scanned points by level 2 walks, so the pattern is not so well defined
 *
 * EXTERNAL FUNCTION AND POINTERS
 *  ipt iup idown (as usual)
 * 
 * memory_map_counter: integer that counts the number of contiguos region of non redundant informations of the total array;
 * memory_map_address: the starting point of every non reduntat zone
 *
 *
 * function_copy_length: integer that counts the number of sites that are redundant
 * function_copy_list_from & function_copy_list_to
 * is the list of sites you have to copy from and to to syconize the array
 *
 * geometry_mpi_blocked: the initialize function
 *
 *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "geometry.h" 
#include "global.h" 
#include "error.h"
#include "logger.h"



#define true 1
#define false 0


#define local_index(nt,nx,ny,nz)  (((nt)+2*T_BORDER+T)%(2*T_BORDER+T)+	\
				   (T+2*T_BORDER)*(((nx)+2*X_BORDER+X)%(2*X_BORDER+X))+ \
				   (T+2*T_BORDER)*(X+2*X_BORDER)*(((ny)+2*Y_BORDER+Y)%(2*Y_BORDER+Y))+ \
				   (T+2*T_BORDER)*(X+2*X_BORDER)*(Y+2*Y_BORDER)*(((nz)+2*Z_BORDER+Z)%(2*Z_BORDER+Z)))

typedef struct
{
  int bt_start;
  int bx_start;
  int by_start;
  int bz_start;
  int bt_width;
  int bx_width;
  int by_width;
  int bz_width;
  int index_start;
  int index_end;
  int level;
  int id_proc; /*id of the processor to which i should send or from whom i would like to recive data*/
} border_id;

static int OVERSIZE_VOLUME;
static int TOTAL_VOLUME;
static int N_BORDER;

static int memory_map_counter=0;
static int local_memory_map_counter=0;
static unsigned int *memory_map_address=NULL;
static unsigned int *memory_map_end=NULL;

static int memory_map_counter_e=0;
static int local_memory_map_counter_e=0;
static unsigned int *memory_map_address_e=NULL;
static unsigned int *memory_map_end_e=NULL;
static int index_counter_e=0;

static int memory_map_counter_o=0;
static int local_memory_map_counter_o=0;
static unsigned int *memory_map_address_o=NULL;
static unsigned int *memory_map_end_o=NULL;
static int index_counter_o=0;

static unsigned int * function_copy_list_from=NULL;
static unsigned int * function_copy_list_to=NULL;
static unsigned int * function_copy_list_len=NULL;
static int function_copy_length=0;

static unsigned int * function_copy_list_from_o=NULL;
static unsigned int * function_copy_list_to_o=NULL;
static unsigned int * function_copy_list_len_o=NULL;
static int function_copy_length_o=0;

static unsigned int * function_copy_list_from_e=NULL;
static unsigned int * function_copy_list_to_e=NULL;
static unsigned int * function_copy_list_len_e=NULL;
static int function_copy_length_e=0;


static int blx_start,bly_start,blz_start,blt_start;
static int blx_width,bly_width,blz_width,blt_width;

static int index_position=0;
static int index_start_buffer=0;
static int index_eval_border=-1;

static int *map_true2oversize=NULL;
static int *map_oversize2true=NULL;
static border_id *border=NULL;



static int site_sign(int i)
{
  int ax,ay,az,at;
  
  at = map_true2oversize[i]%(T+2*T_BORDER);
  ax = (map_true2oversize[i]/(T+2*T_BORDER))%(X+2*X_BORDER);
  ay = (map_true2oversize[i]/((T+2*T_BORDER)*(X+2*X_BORDER)))%(Y+2*Y_BORDER);
  az = map_true2oversize[i]/((T+2*T_BORDER)*(X+2*X_BORDER)*(Y+2*Y_BORDER));
  return ((ax+ay+az+at+T_BORDER+X_BORDER+Y_BORDER+Z_BORDER+PSIGN+1)&1);
}

static int find_change_point(int start, int end)
{
  int ind=end;
  int sign = site_sign(ind);
  while(ind > start  && sign == site_sign(ind)) ind-- ;
  return ++ind;
}

static void fix_geometry_descriptor()
{
  /*Setting glattice values*/
  int i;
  glattice.local_master_pieces=local_memory_map_counter;
  glattice.total_master_pieces=memory_map_counter;
  glattice.master_start=memory_map_address;
  glattice.master_end=memory_map_end;
  glattice.gsize=index_position;

  if(N_BORDER>0){
    glattice.rbuf_len=malloc((N_BORDER)*sizeof(unsigned int));
    error((glattice.rbuf_len==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glattice.sbuf_len=malloc((N_BORDER)*sizeof(unsigned int));
    error((glattice.sbuf_len==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glattice.sbuf_to_proc=malloc((N_BORDER)*sizeof(unsigned int));
    error((glattice.sbuf_to_proc==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	"Cannot allocate memory");
    
    glattice.sbuf_start=malloc((N_BORDER)*sizeof(unsigned int));
    error((glattice.sbuf_start==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glattice.rbuf_from_proc=malloc((N_BORDER)*sizeof(unsigned int));
    error((glattice.rbuf_from_proc==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glattice.rbuf_start=malloc((N_BORDER)*sizeof(unsigned int));
    error((glattice.rbuf_start==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
  }
  glattice.nbuffers = N_BORDER;

  for(i=0;i<N_BORDER;i++)
    {
      (glattice.sbuf_to_proc)[i] = border[i+1].id_proc;
      (glattice.rbuf_from_proc)[i] = border[i+1+N_BORDER].id_proc;
      (glattice.sbuf_start)[i] = border[i+1].index_start;
      (glattice.rbuf_start)[i] = border[i+1+N_BORDER].index_start;
      (glattice.sbuf_len)[i] = (glattice.rbuf_len)[i] = border[i+1].index_end - border[i+1].index_start;

    }

  
  glattice.copy_from=function_copy_list_from;
  glattice.copy_to=function_copy_list_to;
  glattice.copy_len=function_copy_list_len;
  glattice.ncopies=function_copy_length;

  /*Setting glat_even & glat_odd values*/
  if(N_BORDER>0){
    glat_even.rbuf_len=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_even.rbuf_len==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_even.sbuf_len=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_even.sbuf_len==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_even.sbuf_to_proc=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_even.sbuf_to_proc==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_even.sbuf_start=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_even.sbuf_start==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_even.rbuf_from_proc=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_even.rbuf_from_proc==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_even.rbuf_start=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_even.rbuf_start==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_odd.rbuf_len=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_odd.rbuf_len==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_odd.sbuf_len=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_odd.sbuf_len==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_odd.sbuf_to_proc=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_odd.sbuf_to_proc==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	"Cannot allocate memory");
    
    glat_odd.sbuf_start=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_odd.sbuf_start==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_odd.rbuf_from_proc=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_odd.rbuf_from_proc==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
    
    glat_odd.rbuf_start=malloc((N_BORDER)*sizeof(unsigned int));
    error((glat_odd.rbuf_start==NULL),1,"fix_geometry_descriptor [geometry_mpi.c]",
	  "Cannot allocate memory");
  }
  glat_odd.nbuffers = N_BORDER;

  glat_even.nbuffers = N_BORDER;

  for(i=0;i<N_BORDER;i++)
    {
      (glat_even.sbuf_to_proc)[i] = border[i+1].id_proc;
      (glat_even.rbuf_from_proc)[i] = border[i+1+N_BORDER].id_proc;
      (glat_odd.sbuf_to_proc)[i] = border[i+1].id_proc;
      (glat_odd.rbuf_from_proc)[i] = border[i+1+N_BORDER].id_proc;


      if(site_sign(border[i+1].index_start))
	{
	  (glat_even.sbuf_start)[i] = border[i+1].index_start;
	  (glat_odd.sbuf_start)[i] = find_change_point(border[i+1].index_start,border[i+1].index_end-1);
	  (glat_even.sbuf_len)[i] = find_change_point(border[i+1].index_start,border[i+1].index_end-1) - border[i+1].index_start;
	  (glat_odd.sbuf_len)[i] = border[i+1].index_end - find_change_point(border[i+1].index_start,border[i+1].index_end-1) ;
	  
	}
      else
	{
	  (glat_odd.sbuf_start)[i] = border[i+1].index_start;
	  (glat_even.sbuf_start)[i] = find_change_point(border[i+1].index_start,border[i+1].index_end-1);
	  (glat_odd.sbuf_len)[i] = find_change_point(border[i+1].index_start,border[i+1].index_end-1) - border[i+1].index_start;
	  (glat_even.sbuf_len)[i] = border[i+1].index_end - find_change_point(border[i+1].index_start,border[i+1].index_end-1) ;
	}


      if(site_sign(border[i+1+N_BORDER].index_start))
	{
	  (glat_even.rbuf_start)[i] = border[i+1+N_BORDER].index_start;
	  (glat_odd.rbuf_start)[i] = find_change_point(border[i+1+N_BORDER].index_start,border[i+1+N_BORDER].index_end-1);
	  (glat_even.rbuf_len)[i] = (glat_odd.rbuf_start)[i] - border[i+1+N_BORDER].index_start; 
	  (glat_odd.rbuf_len)[i] = border[i+1+N_BORDER].index_end -(glat_odd.rbuf_start)[i];

	}
      else
	{
	  (glat_odd.rbuf_start)[i] = border[i+1+N_BORDER].index_start;
	  (glat_even.rbuf_start)[i] = find_change_point(border[i+1+N_BORDER].index_start,border[i+1+N_BORDER].index_end-1);
	  (glat_odd.rbuf_len)[i] = (glat_even.rbuf_start)[i] - border[i+1+N_BORDER].index_start; 
	  (glat_even.rbuf_len)[i] = border[i+1+N_BORDER].index_end -(glat_even.rbuf_start)[i];

	}
      
   /*    printf("Both index %d sbuf start & len %d %d rbuf start & len %d %d \n",i,(glattice.sbuf_start)[i],(glattice.sbuf_len)[i],(glattice.rbuf_start)[i],(glattice.sbuf_len)[i]); */
 /*****       printf("Even index %d sbuf start & len %d %d rbuf start & len %d %d \n",i,(glat_even.sbuf_start)[i],(glat_even.sbuf_len)[i],(glat_even.rbuf_start)[i],(glat_even.rbuf_len)[i]); */
 /*****       printf("Odd index %d sbuf start & len %d %d rbuf start & len %d %d \n",i,(glat_odd.sbuf_start)[i],(glat_odd.sbuf_len)[i],(glat_odd.rbuf_start)[i],(glat_odd.rbuf_len)[i]); */
     

    }

  
  glat_even.copy_from=function_copy_list_from_e;
  glat_even.copy_to=function_copy_list_to_e;
  glat_even.copy_len=function_copy_list_len_e;
  glat_even.ncopies=function_copy_length_e;

   
  glat_odd.copy_from=function_copy_list_from_o;
  glat_odd.copy_to=function_copy_list_to_o;
  glat_odd.copy_len=function_copy_list_len_o;
  glat_odd.ncopies=function_copy_length_o;

  glat_even.local_master_pieces=local_memory_map_counter_e;
  glat_even.total_master_pieces=memory_map_counter_e;
  glat_even.master_start=memory_map_address_e;
  glat_even.master_end=memory_map_end_e;
  /* glat_even.gsize=index_counter_e; */
  glat_even.gsize=glattice.gsize;

  glat_odd.local_master_pieces=local_memory_map_counter_o;
  glat_odd.total_master_pieces=memory_map_counter_o;
  glat_odd.master_start=memory_map_address_o;
  glat_odd.master_end=memory_map_end_o;
  /* glat_odd.gsize=index_counter_o; */
  glat_odd.gsize=glattice.gsize;

}

static void geometry_mpi_init()
{
  int i,BOR_CUBE,BOR_SQUARE,L3_BORDER;
  
	X_BORDER=(NP_X>1)?BORDERSIZE:0;
	Y_BORDER=(NP_Y>1)?BORDERSIZE:0;
	Z_BORDER=(NP_Z>1)?BORDERSIZE:0;
	T_BORDER=(NP_T>1)?BORDERSIZE:0;

  error(T-2*T_BORDER<0,1,"geometry_mpi_init [geometry_mpi.c]","Too large T Border in the geometry");
  error(X-2*X_BORDER<0,1,"geometry_mpi_init [geometry_mpi.c]","Too large X Border in the geometry");
  error(Y-2*Y_BORDER<0,1,"geometry_mpi_init [geometry_mpi.c]","Too large Y Border in the geometry");
  error(Z-2*Z_BORDER<0,1,"geometry_mpi_init [geometry_mpi.c]","Too large Z Border in the geometry");

  X_EXT=X+2*X_BORDER;
  Y_EXT=Y+2*Y_BORDER;
  Z_EXT=Z+2*Z_BORDER;
  T_EXT=T+2*T_BORDER;


  OVERSIZE_VOLUME=((X+2*X_BORDER)*(Y+2*Y_BORDER)*(Z+2*Z_BORDER)*(T+2*T_BORDER));

  BOR_CUBE=2*(X*Y*Z*T_BORDER+
	      X*Y*Z_BORDER*T+
	      X*Y_BORDER*Z*T+
	      X_BORDER*Y*Z*T);

  BOR_SQUARE=4*(X*Y*Z_BORDER*T_BORDER+
		X*Y_BORDER*Z*T_BORDER+
		X*Y_BORDER*Z_BORDER*T+
		X_BORDER*Y*Z*T_BORDER+
		X_BORDER*Y*Z_BORDER*T+
		X_BORDER*Y_BORDER*Z*T);

   
  TOTAL_VOLUME=(X-2*X_BORDER)*(Y-2*Y_BORDER)*(Z-2*Z_BORDER)*(T-2*T_BORDER)+
    2*BOR_CUBE+2*BOR_SQUARE;
  
  L3_BORDER = 2*( (X_BORDER!=0 ? 1 : 0) + (Y_BORDER!=0 ? 1 : 0)  + (Z_BORDER!=0 ? 1 : 0)  + (T_BORDER!=0 ? 1 : 0) );

  N_BORDER = L3_BORDER*L3_BORDER/2;

  /*   printf("volume di un bordo %d\n",BOR_CUBE/4+BOR_SQUARE/4);   */
  /*   printf("volume totale %d\n",TOTAL_VOLUME); */

  map_true2oversize=malloc(TOTAL_VOLUME*sizeof(int));
  error((map_true2oversize==NULL),1,"geometry_mpi_init [geometry_mpi.c]",
	"Cannot allocate memory");
  
  map_oversize2true=malloc(OVERSIZE_VOLUME*sizeof(int));
  error((map_oversize2true==NULL),1,"geometry_mpi_init [geometry_mpi.c]",
	"Cannot allocate memory");
  border=malloc((2*N_BORDER+1)*sizeof(border_id));
  error((border==NULL),1,"geometry_mpi_init [geometry_mpi.c]",
	"Cannot allocate memory");


  for(i=0;i<OVERSIZE_VOLUME;i++)
    {
      map_oversize2true[i]=-1;
    }

  for(i=0;i<TOTAL_VOLUME;i++)
    {
      map_true2oversize[i]=-1;
    }

}

static void geometry_mpi_finalize()
{

  if(map_true2oversize!=NULL) free(map_true2oversize);
  if(map_oversize2true!=NULL) free(map_oversize2true);
  if(border!=NULL) free(border);   

}



static void  fix_next_neightbours()
{
  int x0,x1,x2,x3,ix;
  for (x3=0;x3<Z+2*Z_BORDER;x3++)
    for (x2=0;x2<Y+2*Y_BORDER;x2++)
      for (x1=0;x1<X+2*X_BORDER;x1++)
	for (x0=0;x0<T+2*T_BORDER;x0++)
	  {
	    
	    ix = map_oversize2true[local_index(x0,x1,x2,x3)];

	    ipt(x0-T_BORDER,x1-X_BORDER,x2-Y_BORDER,x3-Z_BORDER)=ix ; 

	    if(ix != -1)
	      {
		iup(ix,0)=map_oversize2true[local_index(x0+1,x1,x2,x3)];
		idn(ix,0)=map_oversize2true[local_index(x0-1,x1,x2,x3)];
		iup(ix,1)=map_oversize2true[local_index(x0,x1+1,x2,x3)];
		idn(ix,1)=map_oversize2true[local_index(x0,x1-1,x2,x3)];
		iup(ix,2)=map_oversize2true[local_index(x0,x1,x2+1,x3)];
		idn(ix,2)=map_oversize2true[local_index(x0,x1,x2-1,x3)];
		iup(ix,3)=map_oversize2true[local_index(x0,x1,x2,x3+1)];
		idn(ix,3)=map_oversize2true[local_index(x0,x1,x2,x3-1)];
	      }
	   	    
	  }
  
}



static void set_block_start(int pts,int pxs, int pys, int pzs)
{
  blt_start=pts;
  blx_start=pxs;
  bly_start=pys;
  blz_start=pzs;
}

static void set_block_width(int pte, int pxe, int pye, int pze)
{
  blt_width=pte;
  blx_width=pxe;
  bly_width=pye;
  blz_width=pze;
}



static void set_border_pointer(int actualn,int level)
{
  map_true2oversize[index_position]=actualn;
  if( map_oversize2true[actualn]==-1 ) map_oversize2true[actualn]=index_position;
  index_position++;
}

static int check_evaluated_border(int level)
{
  int i,retval,id=CID;
  retval=true;
  for(i=0;i<(2*N_BORDER+1);i++)
    {
      if( border[i].bt_start  == blt_start &&
	  border[i].bx_start  == blx_start &&
	  border[i].by_start  == bly_start &&
	  border[i].bz_start  == blz_start &&
	  border[i].bt_width == blt_width &&
	  border[i].bx_width == blx_width &&
	  border[i].by_width == bly_width &&
	  border[i].bz_width == blz_width )  retval = false;
      if( blt_width*blx_width*bly_width*blz_width == 0 ) retval = false;

    }
  
  if(retval) 
    {
      /*  printf("CALCOLO BORDO L%d %d,%d,%d,%d,\t%d,%d,%d,%d\n",level,blt_start,blx_start,bly_start,blz_start,blt_width,blx_width,bly_width,blz_width); */
      index_eval_border++;
      border[index_eval_border].bt_start = blt_start;
      border[index_eval_border].bx_start = blx_start;
      border[index_eval_border].by_start = bly_start;
      border[index_eval_border].bz_start = blz_start;
      border[index_eval_border].bt_width = blt_width;
      border[index_eval_border].bx_width = blx_width;
      border[index_eval_border].by_width = bly_width;
      border[index_eval_border].bz_width = blz_width;
      border[index_eval_border].level = level;
      border[index_eval_border].index_start = index_position ;
      border[index_eval_border].index_end = index_position +blt_width*blx_width*bly_width*blz_width;

      if(blt_width == T_BORDER )
	{
	  if(blt_start == T_BORDER ) id = proc_dn(id,0);
	  else if(blt_start == T) id = proc_up(id,0);
	}

      if(blx_width == X_BORDER )
	{
	  if(blx_start == X_BORDER ) id = proc_dn(id,1);
	  else if(blx_start == X) id = proc_up(id,1);
	}

      if(bly_width == Y_BORDER )
	{
	  if(bly_start == Y_BORDER ) id = proc_dn(id,2);
	  else if(bly_start == Y) id = proc_up(id,2);
	}

      if(blz_width == Z_BORDER )
	{
	  if(blz_start == Z_BORDER ) id = proc_dn(id,3);
	  else if(blz_start == Z) id = proc_up(id,3);
	}
      
      border[index_eval_border].id_proc = id;
      
    }
  /*   else printf("GIA' CALCOLATO BORDO %d,%d,%d,%d,%d,%d,%d,%d\n",blt_start,blx_start,bly_start,blz_start,blt_width,blx_width,bly_width,blz_width); */
  return retval;
}

static void walk_on_lattice(int level)
{
  int x0,x1,x2,x3;
  if(check_evaluated_border(level))
    {
 /*****       printf("Start of the L%d border (%d,%d,%d,%d) and width (%d,%d,%d,%d)\n",level,blt_start-T_BORDER,blx_start-X_BORDER,bly_start-Y_BORDER,blz_start-Z_BORDER,blt_width,blx_width,bly_width,blz_width); */
      
      
      for (x3=blz_start;x3<blz_start+blz_width;x3++) 
	for (x2=bly_start;x2<bly_start+bly_width;x2++) 
	  for (x1=blx_start;x1<blx_start+blx_width;x1++) 
	    for (x0=blt_start;x0<blt_start+blt_width;x0++)
	      if((x0+x1+x2+x3+T_BORDER+X_BORDER+Y_BORDER+Z_BORDER+PSIGN+1)&1)
		set_border_pointer(local_index(x0,x1,x2,x3),level);
		  
      for (x3=blz_start;x3<blz_start+blz_width;x3++) 
	for (x2=bly_start;x2<bly_start+bly_width;x2++) 
	  for (x1=blx_start;x1<blx_start+blx_width;x1++) 
	    for (x0=blt_start;x0<blt_start+blt_width;x0++)
	      if(!((x0+x1+x2+x3+T_BORDER+X_BORDER+Y_BORDER+Z_BORDER+PSIGN+1)&1) )
		set_border_pointer(local_index(x0,x1,x2,x3),level);
      
      
    }
}

static void fix_buffer()
{
  index_start_buffer=index_position;
  int i,done_border=index_eval_border+1,id;
  int x0,x1,x2,x3;
  
  for(i=1;i<done_border;i++)
    {
      id = CID;
      index_eval_border++;
      
      set_block_width(border[i].bt_width,border[i].bx_width,border[i].by_width,border[i].bz_width);
      
      border[index_eval_border].bt_width= blt_width;
      border[index_eval_border].bx_width= blx_width;
      border[index_eval_border].by_width= bly_width;
      border[index_eval_border].bz_width= blz_width;
      border[index_eval_border].level = border[i].level;
      
      if(blt_width == T_BORDER )
	{
	  if( border[i].bt_start == T_BORDER )
	    {
	      id = proc_up(id,0);
	      border[index_eval_border].bt_start= T+T_BORDER;
	    }
	  else if( border[i].bt_start == T) 
	    {
	      id = proc_dn(id,0);
	      border[index_eval_border].bt_start= 0;
	    }
	}
      else
	border[index_eval_border].bt_start = border[i].bt_start;
      
      if(blx_width == X_BORDER )
	{
	  if( border[i].bx_start == X_BORDER )
	    {
	      id = proc_up(id,1);
	      border[index_eval_border].bx_start= X+X_BORDER;
	    }
	  else if( border[i].bx_start == X) 
	    {
	      id = proc_dn(id,1);
	      border[index_eval_border].bx_start= 0;
	    }
	}
      else
	border[index_eval_border].bx_start = border[i].bx_start;
     
      if(bly_width == Y_BORDER )
	{
	  if( border[i].by_start == Y_BORDER )
	    {
	      id = proc_up(id,2);
	      border[index_eval_border].by_start= Y+Y_BORDER;
	    }
	  else if( border[i].by_start == Y) 
	    {
	      id = proc_dn(id,2);
	      border[index_eval_border].by_start= 0;
	    }
	}
      else
	border[index_eval_border].by_start = border[i].by_start;
     
      if(blz_width == Z_BORDER )
	{
	  if( border[i].bz_start == Z_BORDER )
	    {
	      id = proc_up(id,3);
	      border[index_eval_border].bz_start= Z+Z_BORDER;
	    }
	  else if( border[i].bz_start == Z) 
	    {
	      id = proc_dn(id,3);
	      border[index_eval_border].bz_start= 0;
	    }
	}
      else
	border[index_eval_border].bz_start = border[i].bz_start;
      
      border[index_eval_border].id_proc = id;
      
      set_block_start(border[index_eval_border].bt_start,border[index_eval_border].bx_start,border[index_eval_border].by_start,border[index_eval_border].bz_start);
      
      border[index_eval_border].index_start = index_position ;
      border[index_eval_border].index_end = index_position +blt_width*blx_width*bly_width*blz_width;
      
      /* printf("CALCOLO BUFFER L%d %d,%d,%d,%d\t,%d,%d,%d,%d\n",border[index_eval_border].level,blt_start,blx_start,bly_start,blz_start,blt_width,blx_width,bly_width,blz_width); */
      
      
      for (x3=blz_start;x3<blz_start+blz_width;x3++) 
	for (x2=bly_start;x2<bly_start+bly_width;x2++) 
	  for (x1=blx_start;x1<blx_start+blx_width;x1++) 
	    for (x0=blt_start;x0<blt_start+blt_width;x0++) 
	      if((x0+x1+x2+x3+T_BORDER+X_BORDER+Y_BORDER+Z_BORDER+PSIGN+1)&1)
		set_border_pointer(local_index(x0,x1,x2,x3),border[index_eval_border].level);
      
      
      for (x3=blz_start;x3<blz_start+blz_width;x3++) 
	for (x2=bly_start;x2<bly_start+bly_width;x2++) 
	  for (x1=blx_start;x1<blx_start+blx_width;x1++) 
	    for (x0=blt_start;x0<blt_start+blt_width;x0++)
	      if(!((x0+x1+x2+x3+T_BORDER+X_BORDER+Y_BORDER+Z_BORDER+PSIGN+1)&1) )
		set_border_pointer(local_index(x0,x1,x2,x3),border[index_eval_border].level);
      
      
    }
}


static void set_inner()
{
  
  set_block_width(T-2*T_BORDER,X-2*X_BORDER,Y-2*Y_BORDER,Z-2*Z_BORDER);  
  
  set_block_start(2*T_BORDER,2*X_BORDER,2*Y_BORDER,2*Z_BORDER);
  
  walk_on_lattice(4);

}


static void set_border(int dir){
  
  int id0,id1,id2,id3;
  int int_id0,int_id1,int_id2,int_id3;
  int out_id0,out_id1,out_id2,out_id3;
  
  out_id0=out_id1=out_id2=out_id3=1;
  
  if(dir==0) out_id0=2;
  else if (dir==1) out_id1=2;
  else if (dir==2) out_id2=2;
  else if (dir==3) out_id3=2;

  int ind0[]={T_BORDER,T};
  int ind1[]={X_BORDER,X};
  int ind2[]={Y_BORDER,Y};
  int ind3[]={Z_BORDER,Z};
	    
  int tmp_ind=index_position;

  for(id0=0;id0<out_id0;id0++)
    for(id1=0;id1<out_id1;id1++)
      for(id2=0;id2<out_id2;id2++)
	for(id3=0;id3<out_id3;id3++)
	  {  
	    
	    /* 	    printf("L3\n"); */

	    /* 	    printf("indice di all'interno di border1 pos= %d, diff=%d\n",index_position,index_position-tmp_ind); */
	    tmp_ind=index_position;
	    /*(Level=3)*/
	    
	    if(out_id0==2){
	      set_block_width(T_BORDER,X,Y,Z);  
	      set_block_start(ind0[id0],X_BORDER,Y_BORDER,Z_BORDER);
	    }
	    else if(out_id1==2){
	      set_block_width(T,X_BORDER,Y,Z);  
	      set_block_start(T_BORDER,ind1[id1],Y_BORDER,Z_BORDER);
	    }
	    else if(out_id2==2){
	      set_block_width(T,X,Y_BORDER,Z);  
	      set_block_start(T_BORDER,X_BORDER,ind2[id2],Z_BORDER);
	    }
	    else if(out_id3==2){
	      set_block_width(T,X,Y,Z_BORDER);  
	      set_block_start(T_BORDER,X_BORDER,Y_BORDER,ind3[id3]);
	    }

	    walk_on_lattice(3);
	    
	    /* 	    printf("indice di all'interno di border2 pos= %d, diff=%d\n",index_position,index_position-tmp_ind); */
	    tmp_ind=index_position;
	    /* 	    printf("L2\n"); */
	    
	    
	    /*(Level=2)*/
	    
 	    if(out_id0==1)
	      {
		
		for(int_id0=0;int_id0<2;int_id0++)
		  {
		    if(out_id1==2){
		      set_block_width(T_BORDER,X_BORDER,Y,Z);  
		      set_block_start(ind0[int_id0],ind1[id1],Y_BORDER,Z_BORDER);
		    }
		    if(out_id2==2){
		      set_block_width(T_BORDER,X,Y_BORDER,Z);  
		      set_block_start(ind0[int_id0],X_BORDER,ind2[id2],Z_BORDER);
		    }
		    if(out_id3==2){
		      set_block_width(T_BORDER,X,Y,Z_BORDER);  
		      set_block_start(ind0[int_id0],X_BORDER,Y_BORDER,ind3[id3]);
		    }
		    walk_on_lattice(2);
		  }
	      }
	    
	    /* 	    printf("indice di all'interno di border3 pos= %d, diff=%d\n",index_position,index_position-tmp_ind); */
	    tmp_ind=index_position;
	    
	    
	    
 	    if(out_id1==1)
	      {
	
	
		for(int_id1=0;int_id1<2;int_id1++)
		  {
		    if(out_id0==2){
		      set_block_width(T_BORDER,X_BORDER,Y,Z);  
		      set_block_start(ind0[id0],ind1[int_id1],Y_BORDER,Z_BORDER);
		    }
		    if(out_id2==2){
		      set_block_width(T,X_BORDER,Y_BORDER,Z);  
		      set_block_start(T_BORDER,ind1[int_id1],ind2[id2],Z_BORDER);
		    }
		    if(out_id3==2){
		      set_block_width(T,X_BORDER,Y,Z_BORDER);  
		      set_block_start(T_BORDER,ind1[int_id1],Y_BORDER,ind3[id3]);
		    }

		    walk_on_lattice(2);
		  }
	      }

	    /* 	    printf("indice di all'interno di border4 pos= %d, diff=%d\n",index_position,index_position-tmp_ind); */
	    tmp_ind=index_position;
	    
 	    if(out_id2==1)
	      {
		
		for(int_id2=0;int_id2<2;int_id2++)
		  {
		    
		    if(out_id0==2){
		      set_block_width(T_BORDER,X,Y_BORDER,Z);  
		      set_block_start(ind0[id0],X_BORDER,ind2[int_id2],Z_BORDER);
		    }
		    if(out_id1==2){
		      set_block_width(T,X_BORDER,Y_BORDER,Z);  
		      set_block_start(T_BORDER,ind1[id1],ind2[int_id2],Z_BORDER);
		    }
		    if(out_id3==2){
		      set_block_width(T,X,Y_BORDER,Z_BORDER);  
		      set_block_start(T_BORDER,X_BORDER,ind2[int_id2],ind3[id3]);
		    }
		    
		    walk_on_lattice(2);
		  }
	      }
	    
	    /* 	    printf("indice di all'interno di border5 pos= %d, diff=%d\n",index_position,index_position-tmp_ind); */
	    tmp_ind=index_position;
	    
 	    if(out_id3==1)
	      {
		for(int_id3=0;int_id3<2;int_id3++)
		  {
		    if(out_id0==2){
		      set_block_width(T_BORDER,X,Y,Z_BORDER);  
		      set_block_start(ind0[id0],X_BORDER,Y_BORDER,ind3[int_id3]);
		    }
		    if(out_id1==2){
		      set_block_width(T,X_BORDER,Y,Z_BORDER);  
		      set_block_start(T_BORDER,ind1[id1],Y_BORDER,ind3[int_id3]);
		    }
		    if(out_id2==2){
		      set_block_width(T,X,Y_BORDER,Z_BORDER);  
		      set_block_start(T_BORDER,X_BORDER,ind2[id2],ind3[int_id3]);
		    }
		    walk_on_lattice(2);
		  }
	      }
	    
	    /* 	    printf("indice di all'interno di border6 pos= %d, diff=%d\n",index_position,index_position-tmp_ind); */
	    tmp_ind=index_position;
	    
	    /* 	    printf("fine\n"); */

	  }
}










static void set_memory_order()
{
  int i1,i2=0;
  int *check=malloc(OVERSIZE_VOLUME*sizeof(int));
  unsigned int *oversize_zone_addr_list=malloc(index_position*sizeof(unsigned int));
  unsigned int *oversize_zone_length_list=malloc(index_position*sizeof(unsigned int));

  int counter_zone=0;
  int test_master=1;
  int * tmp_function_copy_list_from=malloc(index_position*sizeof(int));
  int * tmp_function_copy_list_to=malloc(index_position*sizeof(int));
  int * tmp_function_copy_list_len=malloc(index_position*sizeof(int));

  int tmp_function_copy_length=0;
  
  for (i1=0;i1<OVERSIZE_VOLUME;i1++) check[i1]=0;

  /*The inner lattice*/
  oversize_zone_addr_list[counter_zone]=border[counter_zone].index_start;
  oversize_zone_length_list[counter_zone]=border[counter_zone].index_end-border[counter_zone].index_start;
  counter_zone++;
  local_memory_map_counter=counter_zone;
 

 /*****   printf("QUANDO ENTRO QUI GSIZE VALE %d\n",index_position); */

  /*Border & Buffer*/
  for (i1=border[0].index_end;i1<index_position;i1++)
    {

      if( i1==index_start_buffer ) local_memory_map_counter=counter_zone;

      i2 = map_true2oversize[i1];

      if(check[i2]==0)
	{
	  /*zona master*/
	  check[i2]=1;
	  
	  if(test_master==0 && i1!=index_start_buffer )
	    {
	      oversize_zone_length_list[counter_zone-1]++;
	    }
	  else
	    {
	      oversize_zone_addr_list[counter_zone]=i1;
	      oversize_zone_length_list[counter_zone]=1;
	      counter_zone++;
	    }
	  test_master=0;

	}
      else
	{
	  /*zona copia*/

	  if(map_oversize2true[i2]==map_oversize2true[map_true2oversize[i1-1]]+1 && test_master==1)
	    {
	      tmp_function_copy_list_len[tmp_function_copy_length-1]++; 
	    }
	  else
	    {
	      tmp_function_copy_list_from[tmp_function_copy_length]=map_oversize2true[i2];
	      tmp_function_copy_list_to[tmp_function_copy_length]=i1;
	      tmp_function_copy_list_len[tmp_function_copy_length]=1;
	      tmp_function_copy_length++;
	    }
	  test_master=1;

	}

    }
  

  if(counter_zone!=0)
    {
      memory_map_address=malloc(counter_zone*sizeof(unsigned int));
      error((memory_map_address==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 

      memory_map_end=malloc(counter_zone*sizeof(unsigned int)); 
      error((memory_map_end==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 

      for (i1=0;i1<counter_zone;i1++) 
	{ 
	  memory_map_address[i1]=oversize_zone_addr_list[i1]; 
	  memory_map_end[i1]=memory_map_address[i1]+oversize_zone_length_list[i1]-1;
	} 
      memory_map_counter=counter_zone;
 
    }

  if(tmp_function_copy_length!=0)
    {
      function_copy_list_from=malloc(tmp_function_copy_length*sizeof(int));
      error((function_copy_list_from==NULL),1,"set_memory_order [geometry_mpi.c]",
	    "Cannot allocate memory");
    
      function_copy_list_to=malloc(tmp_function_copy_length*sizeof(int));
      error((function_copy_list_to==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 
    
      function_copy_list_len=malloc(tmp_function_copy_length*sizeof(int));
      error((function_copy_list_len==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 
    
    
      for (i1=0;i1<tmp_function_copy_length;i1++) 
	{ 
	  function_copy_list_from[i1]=tmp_function_copy_list_from[i1]; 
	  function_copy_list_to[i1]=tmp_function_copy_list_to[i1]; 
	  function_copy_list_len[i1]=tmp_function_copy_list_len[i1]; 
	} 
      function_copy_length=tmp_function_copy_length;
    }
  /*even odd*/
  
  int *tmp_function_copy_from_e=malloc(index_position*sizeof(int));
  int *tmp_function_copy_to_e=malloc(index_position*sizeof(int));
  int *tmp_function_copy_len_e=malloc(index_position*sizeof(int));

  int tmp_function_copy_length_e=0;
  int *tmp_function_copy_from_o=malloc(index_position*sizeof(int));
  int *tmp_function_copy_to_o=malloc(index_position*sizeof(int));
  int *tmp_function_copy_len_o=malloc(index_position*sizeof(int));

  int tmp_function_copy_length_o=0;
  int start=0;
  int sign,end,pt,length,diff;

  for(i1=0;i1<function_copy_length;i1++)
    {
 /*****       printf("\n\nINDICE COPIA %d \n",i1); */
     
      pt = function_copy_list_from[i1];
      end = function_copy_list_from[i1]  + function_copy_list_len[i1] - 1;
      start=pt;
      diff=length=0;
 /*****       printf("ZONE FROM %d TO %d LENGTH %d\n",pt,function_copy_list_to[i1],function_copy_list_len[i1]); */

      while(pt <= end)
	{
 /***** 	  printf("rumore %d \n",pt); */
	  sign = site_sign(pt);
  	  while(site_sign(pt)==sign && pt <= end)
	    {
 /*****  	      printf("segno %d sito %d\n",site_sign(pt),pt); */
	      pt++;
	    }

	  if((sign)&1)
	    {
	      tmp_function_copy_from_e[tmp_function_copy_length_e]=start;
	      tmp_function_copy_len_e[tmp_function_copy_length_e]=pt-start;
	      tmp_function_copy_to_e[tmp_function_copy_length_e]=function_copy_list_to[i1]+start-function_copy_list_from[i1];
 /*****  	      printf("SCRIVO from %d to %d Len %d E \n",start,function_copy_list_to[i1]+start-function_copy_list_from[i1],pt-start); */
	      tmp_function_copy_length_e++;
	      }
	  else
	    {
	      tmp_function_copy_from_o[tmp_function_copy_length_o]=start;
	      tmp_function_copy_len_o[tmp_function_copy_length_o]=pt-start;
	      tmp_function_copy_to_o[tmp_function_copy_length_o]=function_copy_list_to[i1]+start-function_copy_list_from[i1];
 /*****  	      printf("SCRIVO from %d to %d Len %d O \n",start,function_copy_list_to[i1]+start-function_copy_list_from[i1],pt-start);  */
	      tmp_function_copy_length_o++;
	    }
	  start=pt;
	  pt++;
	}
    }

 /*****   printf("FINE DEL WHILE \n"); */

  if(tmp_function_copy_length_o!=0)
    {
      function_copy_list_from_o=malloc(tmp_function_copy_length_o*sizeof(int));
      error((function_copy_list_from_o==NULL),1,"set_memory_order [geometry_mpi.c]",
	    "Cannot allocate memory");
    
      function_copy_list_to_o=malloc(tmp_function_copy_length_o*sizeof(int));
      error((function_copy_list_to_o==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 
      
      function_copy_list_len_o=malloc(tmp_function_copy_length_o*sizeof(int));
      error((function_copy_list_len_o==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 
    
      
      for (i1=0;i1<tmp_function_copy_length_o;i1++) 
	{ 
	  function_copy_list_from_o[i1]=tmp_function_copy_from_o[i1]; 
	  function_copy_list_to_o[i1]=tmp_function_copy_to_o[i1]; 
	  function_copy_list_len_o[i1]=tmp_function_copy_len_o[i1]; 
	} 
      function_copy_length_o=tmp_function_copy_length_o;
    }
  
   if(tmp_function_copy_length_e!=0)
    {
      function_copy_list_from_e=malloc(tmp_function_copy_length_e*sizeof(int));
      error((function_copy_list_from_e==NULL),1,"set_memory_order [geometry_mpi.c]",
	    "Cannot allocate memory");
    
      function_copy_list_to_e=malloc(tmp_function_copy_length_e*sizeof(int));
      error((function_copy_list_to_e==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 
      
      function_copy_list_len_e=malloc(tmp_function_copy_length_e*sizeof(int));
      error((function_copy_list_len_e==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 
    
      
      for (i1=0;i1<tmp_function_copy_length_e;i1++) 
	{ 
	  function_copy_list_from_e[i1]=tmp_function_copy_from_e[i1]; 
	  function_copy_list_to_e[i1]=tmp_function_copy_to_e[i1]; 
	  function_copy_list_len_e[i1]=tmp_function_copy_len_e[i1]; 
	} 
      function_copy_length_e=tmp_function_copy_length_e;
    }
  
 /*****    printf("FINE ALLOCAZIONI\n"); */

   int counter_e=0,counter_o=0;
   int * tmp_master_start_e=malloc(index_position*sizeof(int));
   int * tmp_master_end_e=malloc(index_position*sizeof(int));
   int * tmp_master_start_o=malloc(index_position*sizeof(int));
   int * tmp_master_end_o=malloc(index_position*sizeof(int));

   start=0;
 

 /***** ANTONIO */


   for(i1=0; i1< memory_map_counter;i1++)
     {
       
 /*****        printf("\n\nINDICE I1 %d Local %d \n",i1,local_memory_map_counter); */

       pt = memory_map_address[i1];
       start=pt;
       end = memory_map_end[i1];
       /*        printf("ZONE START %d END %d\n",pt,end); */
       
       
       while(pt <= end)
	 {
	   sign = site_sign(start);
	   while(site_sign(pt)==sign)
	     {
 /***** 	       int ax,ay,az,at; */
 /***** 	       at = map_true2oversize[pt]%(T+2*T_BORDER); */
 /***** 	       ax = (map_true2oversize[pt]/(T+2*T_BORDER))%(X+2*X_BORDER); */
 /***** 	       ay = (map_true2oversize[pt]/((T+2*T_BORDER)*(X+2*X_BORDER)))%(Y+2*Y_BORDER); */
 /***** 	       az = map_true2oversize[pt]/((T+2*T_BORDER)*(X+2*X_BORDER)*(Y+2*Y_BORDER)); */
 /*****  	       printf("segno %d sito %d coord(%d,%d,%d,%d)\n",site_sign(pt),pt,at-T_BORDER,ax-X_BORDER,ay-Y_BORDER,az-Z_BORDER); */
	       pt++;
	       if(pt == end+1)  break;
	     }
	   

	   if((sign)&1)
	     {
 /*****    	       printf("SCRIVO sito %d-%d index E %d\n",start,pt-1,counter_e); */
	       tmp_master_start_e[counter_e]=start;
	       tmp_master_end_e[counter_e]=pt-1;     
	       index_counter_e += tmp_master_end_e[counter_e]-tmp_master_start_e[counter_e]+1;	       
 /*****    	       printf("COUNTER E TOTALE %d PARZIALE %d \n",index_counter_e,tmp_master_end_e[counter_e]-tmp_master_start_e[counter_e]+1); */
	       counter_e++;
	     }
	   else
	     {
 /*****   	       printf("SCRIVO sito %d-%d index O %d\n",start,pt-1,counter_o); */
	       tmp_master_start_o[counter_o]=start;
	       tmp_master_end_o[counter_o]=pt-1;     
	       index_counter_o += tmp_master_end_o[counter_o]-tmp_master_start_o[counter_o]+1;	       
 /***** 	       printf("COUNTER O TOTALE %d PARZIALE %d \n",index_counter_o,tmp_master_end_o[counter_o]-tmp_master_start_o[counter_o]+1); */
	       counter_o++;
	     }
	   start=pt;
	 }

       if(i1==0 && counter_e==0)
	 {
	   tmp_master_start_e[counter_e]=start;
	   tmp_master_end_e[counter_e]=start-1;    
	   index_counter_e += tmp_master_end_e[counter_e]-tmp_master_start_e[counter_e]+1;
	   counter_e++;
	 }
       
       if(i1==0 && counter_o==0)
	 {
	   tmp_master_start_o[counter_o]=start;
	   tmp_master_end_o[counter_o]=start-1;    
	   index_counter_o += tmp_master_end_o[counter_o]-tmp_master_start_o[counter_o]+1;
	   counter_o++;
	 }

       
       if(i1+1==local_memory_map_counter) 
	 {
	   local_memory_map_counter_e=counter_e;
	   
	   local_memory_map_counter_o=counter_o;
	   /*   	   printf("INDICE Local E %d ed O %d GLOBAL %d \n",local_memory_map_counter_e,local_memory_map_counter_o,local_memory_map_counter); */
	 }
       
     }

 /*****   printf("ALLOCAZIONI EO\n"); */
  if(counter_o!=0)
    {
      memory_map_address_o=malloc(counter_o*sizeof(unsigned int));
      error((memory_map_address_o==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 

      memory_map_end_o=malloc(counter_o*sizeof(unsigned int)); 
      error((memory_map_end_o==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 

      for (i1=0;i1<counter_o;i1++) 
	{ 
	  memory_map_address_o[i1]=tmp_master_start_o[i1]; 
	  memory_map_end_o[i1]=tmp_master_end_o[i1];
	} 
      memory_map_counter_o=counter_o;
 
    }
   
   if(counter_e!=0)
    {
      memory_map_address_e=malloc(counter_e*sizeof(unsigned int));
      error((memory_map_address_e==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 

      memory_map_end_e=malloc(counter_e*sizeof(unsigned int)); 
      error((memory_map_end_e==NULL),1,"set_memory_order [geometry_mpi.c]", 
	    "Cannot allocate memory"); 

      for (i1=0;i1<counter_e;i1++) 
	{ 
	  memory_map_address_e[i1]=tmp_master_start_e[i1]; 
	  memory_map_end_e[i1]=tmp_master_end_e[i1];
	} 
      memory_map_counter_e=counter_e;
 
    }
   
 /*****     printf("FINE ALLOCAZIONI EO\n"); */
   free(oversize_zone_addr_list);
   free(oversize_zone_length_list);
   
   free(tmp_function_copy_list_from);
   free(tmp_function_copy_list_to);
   free(tmp_function_copy_list_len);
   free(check);
   free(tmp_function_copy_from_e);
   free(tmp_function_copy_to_e);
   free(tmp_function_copy_len_e);
   free(tmp_function_copy_from_o);
   free(tmp_function_copy_to_o);
   free(tmp_function_copy_len_o);
   free(tmp_master_start_e);
   free(tmp_master_end_e);
   free(tmp_master_start_o);
   free(tmp_master_end_o);

   
}




void geometry_mpi_eo(void)
{
  
  geometry_mpi_init();

  set_inner();

  set_border(0);
  
  set_border(1);
  
  set_border(2);
  
  set_border(3);

  fix_buffer(); 
    
  set_memory_order();

  fix_geometry_descriptor(); 
  
  geometry_mem_alloc();
  
  fix_next_neightbours(); 
 
  geometry_mpi_finalize(); 
 
}












