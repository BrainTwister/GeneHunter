cmake_minimum_required(VERSION 3.0)
project(GeneHunter VERSION 1.1.0 LANGUAGES CXX)
message("${PROJECT_NAME} version ${PROJECT_VERSION}")

# add own cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

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

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fopenmp")

# For adressing an array a 'unsigned char' must be used to avoid negativ positions.
# It would be more portable to use 'unsigned char' instead of 'signed char' directly within the code.
# Since std::string is using by default a 'signed char', many cast would be needed.
# Therefore, at the moment we simply switch off the warning and use '-funsigned-char' for the gnu compiler.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-char-subscripts -Wno-sign-compare")

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_PROFILING "-O3 -pg")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")

# enabling coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

add_definitions(-DGENEHUNTER_THROW_FATAL_ERROR -DGENEHUNTER_USE_BOOST_FILESYSTEM)

# C++ ISO standard
set(CPP_ISO_STANDARD "c++14" CACHE STRING "C++ ISO standard")
message("Use ${CPP_ISO_STANDARD} ISO standard")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CPP_ISO_STANDARD}")

if( ${CMAKE_CXX_COMPILER} MATCHES "icpc" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GXX_EXPERIMENTAL_CXX0X__ -DNO_INITIALIZER_LIST_SUPPORTED")
endif()

if( ${CMAKE_COMPILER_IS_GNUCXX} )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=10000 -funsigned-char")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -funroll-loops -ffast-math -mfpmath=sse -march=core2 -mtune=core2 -msse2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops --param max-unroll-times=100 -ffast-math -mfpmath=sse -march=core2 -mtune=core2 -msse2")
endif()

set(CMAKE_VERBOSE_MAKEFILE false)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/UtilitiesLib/Version.h.in"
  "${PROJECT_SOURCE_DIR}/src/UtilitiesLib/Version.h"
)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

FIND_PACKAGE(Boost COMPONENTS filesystem iostreams serialization system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(-DUSE_BOOST -DHAVE_SHARED_POINTER)
endif(Boost_FOUND)

find_package(MySQL REQUIRED)
find_package(ZLIB REQUIRED)

add_subdirectory(ArgumentParser)
add_subdirectory(src)

find_package(GTest)
if(GTEST_FOUND)
    ENABLE_TESTING()
    add_subdirectory(test)
endif(GTEST_FOUND)

FIND_PACKAGE(Doxygen)
if(DOXYGEN_FOUND)

    set(doxyfile_in ${PROJECT_SOURCE_DIR}/doxygen/Doxyfile.in)
    set(doxyfile ${PROJECT_SOURCE_DIR}/doxygen/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doxygen
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/doxygen/html DESTINATION share/doc)
    
endif(DOXYGEN_FOUND)
