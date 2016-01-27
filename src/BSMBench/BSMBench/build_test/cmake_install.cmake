# Install script for directory: /home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Include/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Geometry/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/IO/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Memory/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Observables/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Random/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Update/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Utils/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Error/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Inverters/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Make/cmake_install.cmake")
  include("/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/Bench/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "/home/sid/Work/DiRAC3-testsuite/src/BSMBench/BSMBench/build_test/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
