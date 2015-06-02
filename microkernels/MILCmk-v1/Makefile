CC = mpixlc_r
#COPT = -O3 -qhot -qsimd=noauto
COPT = -O5 -qhot=level=2 -qsimd=auto
OMP = -qsmp=omp

QLADIR = qla-1.7.1
CINC = -I$(QLADIR)
CFLAGSA = -DHAVE_XLC
CFLAGSF = $(CFLAGSA) $(COPT) $(OMP) $(CINC) -DQLA_Precision=\'F\' -DQLA_Colors=3 -DQLA_Nc=3
CFLAGSD = $(CFLAGSA) $(COPT) $(OMP) $(CINC) -DQLA_Precision=\'D\' -DQLA_Colors=3 -DQLA_Nc=3

FSRC = $(wildcard $(QLADIR)/QLA_F3_*.c)
FOBJS = $(patsubst %.c, %.o, $(FSRC))
DSRC = $(wildcard $(QLADIR)/QLA_D3_*.c)
DOBJS = $(patsubst %.c, %.o, $(DSRC))

PROGS = qla_bench-$(QLADIR)-f3 qla_bench-$(QLADIR)-d3

all: $(PROGS)

qla_bench-$(QLADIR)-f3: qla_bench.c $(FOBJS)
	$(CC) $(CFLAGSF) $< $(FOBJS) -o $@

qla_bench-$(QLADIR)-d3: qla_bench.c $(DOBJS)
	$(CC) $(CFLAGSD) $< $(DOBJS) -o $@

$(FOBJS):%.o:%.c
	$(CC) $(CFLAGSF) -c $< -o $@

$(DOBJS):%.o:%.c
	$(CC) $(CFLAGSD) -c $< -o $@

clean:
	rm -f $(PROGS) *.o $(QLADIR)/*.o

tarball:
	tar jcvhf MILCmk-v1.tar.bz2 --transform 's|^./|MILCmk-v1/|' ./README ./Makefile ./runtests ./bench-*.output ./*.[ch] ./qla-1.7.1/*.[ch]
