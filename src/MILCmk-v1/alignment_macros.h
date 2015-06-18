#ifndef _COMPILER_ALIGNMENT_MACROS_
#define _COMPILER_ALIGNMENT_MACROS_

// GCC aligned
#ifdef __GNUC__
  #define __alignx(a,p) gcc_aligned(a,p)
  void gcc_aligned(size_t a, void* p);
#endif

#endif