cmake_minimum_required(VERSION 3.0)
project(ArgumentParser VERSION 1.3.0 LANGUAGES CXX)
message("${PROJECT_NAME} version ${PROJECT_VERSION}")

# add own cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/")

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory)
          and run CMake from there. You may need to remove CMakeCache.txt.")
endif()

# guard against bad build-type strings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if(    NOT cmake_build_type_tolower STREQUAL "release"
   AND NOT cmake_build_type_tolower STREQUAL "debug"
   AND NOT cmake_build_type_tolower STREQUAL "profiling"
   AND NOT cmake_build_type_tolower STREQUAL "relwithdebinfo")
  message(FATAL_ERROR "Unknown build type \"${CMAKE_BUILD_TYPE}\". Allowed values are Release, Debug, Profiling, RelWithDebInfo (case-insensitive).")
endif()

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_PROFILING "-O3 -pg")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

set(CMAKE_VERBOSE_MAKEFILE false)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)

# Google C++ unit test framework
FIND_PACKAGE(GTest)
if(GTEST_FOUND)
    ENABLE_TESTING()
    add_subdirectory(test)
endif(GTEST_FOUND)
