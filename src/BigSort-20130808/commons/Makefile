
CLUSTER = Magellan
CLUSTER = Mira
CLUSTER = Vesta

HOST := $(shell hostname | cut -d . -f 1)
IP := $(shell ifconfig eth0|grep inet | grep -v inet6 | awk '{print $$2}' | sed 's/addr://')
ifeq ($(HOST), vestalac1)
  CLUSTER = Vesta
else
      ifeq ($(HOST), miralac1)
        CLUSTER = Mira
      else
        ifeq ($(IP), 10.238.116.3)
          CLUSTER = Magellan
        endif
      endif
endif

ifeq ($(CLUSTER), Magellan)
  SCRATCH_DIR = /gpfs/mira-fs0/projects/Performance/jmeng
  CPP_BIN	= mpicxx
endif

ifeq ($(CLUSTER),$(filter $(CLUSTER), Mira Vesta))
  ifeq ($(CLUSTER), Mira)
    SCRATCH_DIR = /gpfs/mira-fs0/projects/Performance/jmeng
  endif
  ifeq ($(CLUSTER), Vesta)
    SCRATCH_DIR = /gpfs/vesta-fs0/projects/Performance/jmeng
  endif

  MPIHPM_LIB = ~morozov/HPM/lib
  BGPM_LIB = /bgsys/drivers/ppcfloor/bgpm/lib/libbgpm.a
  CPP_BIN = mpixlcxx_r
  ifeq ($(OPENMP), yes)
    OPENMP = -qsmp=omp
    LIB_MPIHPM = libmpihpm_smp.a
  else
    OPENMP = -qnosmp
    LIB_MPIHPM = libmpihpm.a
  endif
  ifeq ($(PROF), yes)
    #libs = /home/morozov/HPM/lib/libmpihpm_smp.a /bgsys/drivers/ppcfloor/bgpm/lib/libbgpm.a #/home/morozov/fixes/libc.a
    #libs = $(MPIHPM_LIB)/libmpihpm_smp.a $(BGPM_LIB) #/home/morozov/fixes/libc.a
    libs = $(MPIHPM_LIB)/$(LIB_MPIHPM) $(BGPM_LIB) #/home/morozov/fixes/libc.a
    PROF_FLAGS = -pg -g
  endif
endif

CPP_SRC	= $(wildcard *.cpp)
OBJ	= $(CPP_SRC:.cpp=.o)

