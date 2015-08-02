set(CMAKE_CXX_COMPILER "/opt/cray/craype/2.2.1/bin/CC")
set(CMAKE_CXX_COMPILER_ARG1 "")
set(CMAKE_CXX_COMPILER_ID "Cray")
set(CMAKE_CXX_COMPILER_VERSION "8.3")
set(CMAKE_CXX_PLATFORM_ID "Linux")

set(CMAKE_AR "/opt/cray/cce/8.3.7/cray-binutils/x86_64-unknown-linux-gnu/bin/ar")
set(CMAKE_RANLIB "/opt/cray/cce/8.3.7/cray-binutils/x86_64-unknown-linux-gnu/bin/ranlib")
set(CMAKE_LINKER "/opt/cray/cce/8.3.7/cray-binutils/x86_64-unknown-linux-gnu/bin/ld")
set(CMAKE_COMPILER_IS_GNUCXX )
set(CMAKE_CXX_COMPILER_LOADED 1)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_CXX_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_CXX_COMPILER_ENV_VAR "CXX")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_CXX_COMPILER_ID_RUN 1)
set(CMAKE_CXX_IGNORE_EXTENSIONS inl;h;hpp;HPP;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS C;M;c++;cc;cpp;cxx;m;mm;CPP)
set(CMAKE_CXX_LINKER_PREFERENCE 30)
set(CMAKE_CXX_LINKER_PREFERENCE_PROPAGATES 1)

# Save compiler ABI information.
set(CMAKE_CXX_SIZEOF_DATA_PTR "8")
set(CMAKE_CXX_COMPILER_ABI "ELF")
set(CMAKE_CXX_LIBRARY_ARCHITECTURE "")

if(CMAKE_CXX_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_CXX_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_CXX_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_CXX_COMPILER_ABI}")
endif()

if(CMAKE_CXX_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()




set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "AtpSigHandler;AtpSigHCommData;sci_cray_mpi_mp;sci_cray_mp;mpichcxx_cray;mpich_cray;pgas-dmapp;cray-c++-rts;craystdc++;xpmem;dmapp;pmi;udreg;alpslli;alpsutil;rca;wlm_detect;ugni;omp;craymp;modules;fi;f;pthread;craymath;m;gfortran;quadmath;u;rt;csup;cray-c++-rts;craystdc++;supc++;tcmalloc_minimal;stdc++;pthread;c;csup;m")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "/opt/cray/cce/8.3.7/CC/x86-64/lib/x86-64;/opt/gcc/4.8.1/snos/lib64;/opt/cray/libsci/13.0.1/CRAY/83/sandybridge/lib;/opt/cray/dmapp/default/lib64;/opt/cray/mpt/7.1.1/gni/mpich2-cray/83/lib;/opt/cray/rca/1.0.0-2.0501.48090.7.46.ari/lib64;/opt/cray/alps/5.1.1-2.0501.8507.1.1.ari/lib64;/opt/cray/xpmem/0.1-2.0501.48424.3.3.ari/lib64;/opt/cray/dmapp/7.0.1-1.0501.8315.8.4.ari/lib64;/opt/cray/pmi/5.0.6-1.0000.10439.140.2.ari/lib64;/opt/cray/ugni/5.0-1.0501.8253.10.22.ari/lib64;/opt/cray/udreg/2.3.2-1.0501.7914.1.13.ari/lib64;/opt/cray/atp/1.7.5/lib;/opt/cray/cce/8.3.7/craylibs/x86-64;/opt/cray/wlm_detect/1.0-1.0501.47908.2.2.ari/lib64;/opt/gcc/4.8.1/snos/lib/gcc/x86_64-suse-linux/4.8.1;/opt/cray/cce/8.3.7/cray-binutils/x86_64-unknown-linux-gnu/lib;//usr/lib64")
set(CMAKE_CXX_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")



