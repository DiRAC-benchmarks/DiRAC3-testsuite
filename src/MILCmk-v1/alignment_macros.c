#include <stdlib.h>
#include "alignment_macros.h"

// GCC aligned
#ifdef __GNUC__
  #define MILC_GCC_ALIGNMENT 16
  void gcc_aligned(size_t a, void* p) {p=__builtin_assume_aligned(p,MILC_GCC_ALIGNMENT);}
#endif
