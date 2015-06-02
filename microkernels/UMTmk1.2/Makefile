#BHEADER****************************************************************
# (c) 2006   The Regents of the University of California               *
#                                                                      *
# See the file COPYRIGHT_and_DISCLAIMER for a complete copyright       *
# notice and disclaimer.                                               *
#                                                                      *
#EHEADER****************************************************************

F90      = mpiifort

CC       = mpiicc

LDR      = mpiifort

#FLOOR     = /bgsys/drivers/ppcfloor

#INCDIR    =  -I$(FLOOR) -I/bgsys/drivers/ppcfloor/spi/include/kernel/cnk

# FFLAGS   = -fno-inline-functions -no-ip -g -c -O2 -vec-report2 
FFLAGS   = -fno-inline-functions -no-ip -g -c -O2 -vec-report2 

CFLAGS   = -fno-inline-functions -no-ip -g -c -O2 -vec-report2 -xAVX

LDFLAGS  = -g -openmp -no-ip 
 
LIBS     = 
LIB_DIRS = 
PROG     = UMTmk

OBJS     = kind_mod.o constant_mod.o size_mod.o zonedata_mod.o geometry_mod.o                  snswp3d.o main.o

all : $(PROG)

$(PROG) : $(OBJS)
	$(LDR)  $(LDFLAGS) -o $(PROG) $(OBJS) $(LIB_DIRS) $(LIBS)

clean :
	rm -f *.o *.mod $(PROG) batch.* core job.out *.lst *~ 


.SUFFIXES : .mod .o .F90 .c

#*****************************************************************
#* Rules for Fortran and C object file dependencies
#*****************************************************************
.F90.o :
	$(F90) $(FFLAGS) $*.F90 

.c.o :
	$(CC) $(CFLAGS) $*.c 

.F90.mod :
	$(F90) $(FFLAGS) $*.F90 




