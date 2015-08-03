
include ../commons/Makefile

#2048 GB
NUM = 274877906944
DRAM_ALLOC = 8589934592L
FILE = dat
NODES = 128

# 64 KB
NUM = 8192
DRAM_ALLOC = 1024L
FILE = dat
NODES = 16

# 256 MB
NUM = 16777216
DRAM_ALLOC = 524288L
FILE = dat
NODES = 64

# ====================
DRAM_ALLOC = 4194304L
FILE = dat
NODES = 128
# 1 GB
NUM = 134217728
# 2 GB
NUM = 268435456
# 4 GB
NUM = 536870912
# 8 GB
NUM = 1073741824
# 16 GB
NUM = 2147483648
# 32 GB
NUM = 4294967296
# 64 GB
NUM = 8589934592
# 128 GB
NUM = 17179869184
# 256 GB
NUM = 34359738368
# 512 GB
NUM = 68719476736

QUEUE_CMD = qsub
CORES = 1
TIME = 60
PROJECT = Performance
ENV = --env VPROF_PROFILE=yes

OPT = 2

BINARY = chkorder

CFLAGS += -O$(OPT)

CXX = $(CPP_BIN)
LD = $(CPP_BIN)

.c.o :
	$(CC) $(CFLAGS) $< -c

.cpp.o :
	$(CXX) $(CFLAGS) $< -c

.f.o :
	$(F90_BIN) $(CFLAGS) $< -c

.c.s :
	$(CC) $(CFLAGS) $< -S

.cpp.s :
	$(CXX) $(CFLAGS) $< -S

.f.s :
	$(F90_BIN) $(CFLAGS) $< -S


all: $(OBJ)
	$(LD) $(OBJ) -o $(BINARY)

clean:
	rm -f $(OBJ) $(BINARY) *.error *.output *.cobaltlog
  
qsub: $(BINARY)
	#$(QUEUE_CMD) -n $(NODES) --mode c$(CORES) -t $(TIME) -A $(PROJECT) $(BINARY) $(NUM) $(DRAM_ALLOC) $(SCRATCH_DIR)/sorted.$(NUM).$(FILE)
	$(QUEUE_CMD) -n $(NODES) --mode c$(CORES) -t $(TIME) -A $(PROJECT) $(BINARY) $(NUM) $(DRAM_ALLOC) $(SCRATCH_DIR)/data/$(NUM)_unsorted.$(FILE)
	#$(QUEUE_CMD) -n $(NODES) --mode c$(CORES) -t $(TIME) -A $(PROJECT) $(BINARY) $(NUM) $(DRAM_ALLOC) $(SCRATCH_DIR)/data/$(NUM)_unsorted_little_endian.$(FILE)

