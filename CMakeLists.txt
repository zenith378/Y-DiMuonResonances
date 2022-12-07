cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ComputingMethodsCorCar)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

foreach(path ${CMAKE_PREFIX_PATH})
  message("Path: " ${path})
endforeach(path)

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Hist Tree Net RooFit RooStats RooFitCore)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
include_directories(./include/)
include_directories(./src/)
add_definitions(${ROOT_CXX_FLAGS})

ROOT_GENERATE_DICTIONARY(G__CorCar include/Cuts.h include/df_set.h include/SpectrumPlot.h include/fitRoo.h)

#---Create a shared library with geneated dictionary
add_library(libCorCar SHARED src/Cuts.C src/df_set.C src/SpectrumPlot.C src/fitRoo.C)


target_link_libraries(libCorCar ${ROOT_LIBRARIES})

#---Create  a main program using the library
add_executable(${PROJECT_NAME} src/main.C)
target_link_libraries(${PROJECT_NAME} libCorCar ${ROOT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} libCorCar PROPERTY CXX_STANDARD 17)