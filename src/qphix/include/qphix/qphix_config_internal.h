/* include/qphix/qphix_config_internal.h.in.  Generated from configure.ac by autoheader.  */

/* Name of package */
#define PACKAGE qphix

/* Define to the address where bug reports for this package should be sent. */

/* Define to the full name of this package. */
#define PACKAGE_NAME qphix

/* Define to the full name and version of this package. */
#define PACKAGE_STRING qphix

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME qphix

/* Define to the home page for this package. */

/* Define to the version of this package. */

/* Generate AVX Specializations */
#ifdef QPHIX_AVX_SOURCE
#define QPHIX_AVX_SOURCE 
#endif

/* Build Clover Term */
#ifdef QPHIX_BUILD_CLOVER
#define QPHIX_BUILD_CLOVER 
#endif

/* Do the actual comms */
#ifdef QPHIX_DO_COMMS
#define QPHIX_DO_COMMS 
#endif

/* Fake Comms */
#ifdef QPHIX_FAKE_COMMS
#define QPHIX_FAKE_COMMS 
#endif


/* L2 Cache alignment */
#ifdef QPHIX_LLC_CACHE_ALIGN
#define QPHIX_LLC_CACHE_ALIGN 
#endif

/* Generate MIC Specializations */
#ifdef QPHIX_MIC_SOURCE
#define QPHIX_MIC_SOURCE 
#endif
/* Parscalar Arch */

#ifdef QPHIX_PARSCALAR
#define QPHIX_PARSCALAR 
#endif

/* QMP Comms */

#ifdef QPHIX_QMP_COMMS
#define QPHIX_QMP_COMMS 
#endif

/* Generate QPX Specializations */
#ifdef QPHIX_QPX_SOURCE
#define QPHIX_QPX_SOURCE 
#endif

/* Scalar Arch */

#ifdef QPHIX_SCALAR
#define QPHIX_SCALAR 
#endif

/* Generate Scalar Specializations */
#ifdef QPHIX_SCALAR_SOURCE
#define QPHIX_SCALAR_SOURCE 
#endif

/* SOALEN is 16 */
#ifdef QPHIX_SOALEN
#define QPHIX_SOALEN 
#endif

/* CEAN is on */
#ifdef QPHIX_USE_CEAN
#define QPHIX_USE_CEAN 
#endif

/* MM_MALLOC is on */

#ifdef QPHIX_USE_MM_MALLOC
#define QPHIX_USE_MM_MALLOC 
#endif
/* Version number of package */

#define VERSION 3.1.3
