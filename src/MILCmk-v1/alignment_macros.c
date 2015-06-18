#include <stdlib.h>
#include "alignment_macros.h"

// GCC aligned
#ifdef __GNUC__
  void gcc_aligned(size_t a, void* p) {p=__builtin_assume_aligned(p,a);}
#endif