
#--- ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

foreach(path ${CMAKE_PREFIX_PATH})
  message("Path: " ${path})
endforeach(path)

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Hist Tree Net RooFit RooStats RooFitCore)


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

add_definitions(${ROOT_CXX_FLAGS})


ROOT_GENERATE_DICTIONARY(G__YCrossFit include/Cuts.h include/df_set.h include/SpectrumPlot.h include/fitRoo.h include/optionParse.h include/diffCrossSection.h)

# Add a library with the above sources
add_library(YCrossFit src/Cuts.C src/df_set.C src/fitRoo.C src/optionParse.C src/SpectrumPlot.C src/diffCrossSection.C)

target_link_libraries(YCrossFit ${ROOT_LIBRARIES})

add_library(sub::lib1 ALIAS YCrossFit)

target_include_directories( YCrossFit
    PUBLIC ${PROJECT_SOURCE_DIR}/YCrossFit/include
    ${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS}
)
set_property(TARGET YCrossFit PROPERTY CXX_STANDARD 17)
