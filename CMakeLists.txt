project(GeneAssembler CXX)

cmake_minimum_required(VERSION 2.6.2)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# guard against in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

# guard against bad build-type strings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if(    NOT cmake_build_type_tolower STREQUAL "debug"
   AND NOT cmake_build_type_tolower STREQUAL "release"
   AND NOT cmake_build_type_tolower STREQUAL "relwithdebinfo"
   AND NOT cmake_build_type_tolower STREQUAL "profiling" )
  message(FATAL_ERROR "Unknown build type \"${CMAKE_BUILD_TYPE}\". Allowed values are Debug, Release, RelWithDebInfo, Profiling (case-insensitive).")
endif()

set(CMAKE_CXX_FLAGS "-std=c++0x -fPIC -fopenmp -DBOOST_DATE_TIME_POSIX_TIME_STD_CONFIG -DGENEASSEMBLER_VERSION=\"1.0 rev 455\" -DGENEASSEMBLER_THROW_FATAL_ERROR")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG_MODE")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DDEBUG_MODE")
set(CMAKE_CXX_FLAGS_PROFILING "-O3 -pg")

if( ${CMAKE_CXX_COMPILER} MATCHES "icpc" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__GXX_EXPERIMENTAL_CXX0X__ -DNO_INITIALIZER_LIST_SUPPORTED")
endif()

if( ${CMAKE_COMPILER_IS_GNUCXX} )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-depth=10000 -funsigned-char")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -funroll-loops -ffast-math -mfpmath=sse -march=core2 -mtune=core2 -msse2 -DBLASBOOSTER_DEBUG_MODE")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops --param max-unroll-times=100 -ffast-math -mfpmath=sse -march=core2 -mtune=core2 -msse2")
endif()

set(CMAKE_VERBOSE_MAKEFILE false)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

ENABLE_TESTING()

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
    
FIND_PACKAGE(Boost COMPONENTS filesystem serialization system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(-DUSE_BOOST -DHAVE_SHARED_POINTER)
endif(Boost_FOUND)

FIND_PACKAGE(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${PROJECT_SOURCE_DIR}/doxygen/Doxyfile
        ${PROJECT_BINARY_DIR}/doxygen/Doxyfile
    )
    add_custom_target(
    	doc
    	${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/doxygen/Doxyfile
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doxygen
            COMMENT "Generating API documentation with Doxygen" VERBATIM
    )    
endif(DOXYGEN_FOUND)

find_package(MySQL REQUIRED)

add_subdirectory(src/DatabaseBuilder)
add_subdirectory(src/CDSDatabaseBuilder)
add_subdirectory(src/GeneAssembler)
add_subdirectory(src/GenomeLib)
add_subdirectory(src/UtilitiesLib)
add_subdirectory(src/TaxonomyDatabaseBuilder)

find_package(GTest)
if(GTEST_FOUND)
    add_subdirectory(src/GenomeTest)
    add_subdirectory(src/UtilitiesTest)
endif(GTEST_FOUND)
