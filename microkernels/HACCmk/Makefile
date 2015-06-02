
# bgq, intel
PLAT = bgq

ifeq "$(PLAT)" "bgq"
CC = bgxlc_r
LD = $(CC) 
LIBS = -L$(HOME)/lib -ltimebase
CFLAGS_KERNEL = -O3 -qhot=level=1 -qsimd=auto -qreport
CFLAGS_MAIN   = -DTIMEBASE -O0 -qsmp=omp:noauto -qreport
LDFLAGS       = -O0 -qsmp 
endif

ifeq "$(PLAT)" "intel"
CC = icc
LD = $(CC) 
LIBS = -L$(HOME)/lib -ltimebase
CFLAGS_KERNEL = -O3 -xAVX -no-prec-div -vec-report6 -opt-report3 -Wall
CFLAGS_MAIN   = -DTIMEBASE -O0 -openmp -openmp-report2 -Wall -w3
LDFLAGS       = -O0 -openmp -static
endif



# not used if the target platform defined correctly
CFLAGS = -O2


OBJ = main.o Step10_orig.o mysecond.o

all: main

main.o : main.c
	$(CC) $(CFLAGS_MAIN) -c $< -o $@

mysecond.o : mysecond.c
	$(CC) -O0 -c -o mysecond.o mysecond.c

Step10_orig.o : Step10_orig.c
	$(CC) $(CFLAGS_KERNEL) -c $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $(INCLUDE) $<

%.o : %.s
	$(FF) $(FFLAGS) -c $(INCLUDE) $<


main: $(OBJ)
	$(LD) $(LDFLAGS) -o HACCmk $(OBJ) $(LIBS)

clean:
	rm -f $(OBJ) HACCmk *.lst *.error *.cobaltlog
 
run: $(TARGET)$(EXE)
	./$(TARGET)$(EXE)


