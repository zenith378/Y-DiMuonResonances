
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

foreach(path ${CMAKE_PREFIX_PATH})
  message("Path: " ${path})
endforeach(path)

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Hist Tree Net RooFit RooStats RooFitCore)


#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})

#include_directories(./include/)

add_definitions(${ROOT_CXX_FLAGS})


ROOT_GENERATE_DICTIONARY(G__YFitTests test0.C  
${PROJECT_SOURCE_DIR}/YFit/include/Cuts.h 
${PROJECT_SOURCE_DIR}/YFit/include/df_set.h 
${PROJECT_SOURCE_DIR}/YFit/include/SpectrumPlot.h 
${PROJECT_SOURCE_DIR}/YFit/include/fitRoo.h 
${PROJECT_SOURCE_DIR}/YFit/include/optionParse.h)

add_library(YFitTests test0.C 
${PROJECT_SOURCE_DIR}/YFit/src/Cuts.C 
${PROJECT_SOURCE_DIR}/YFit/src/df_set.C 
${PROJECT_SOURCE_DIR}/YFit/src/fitRoo.C 
${PROJECT_SOURCE_DIR}/YFit/src/optionParse.C 
${PROJECT_SOURCE_DIR}/YFit/src/SpectrumPlot.C
)
add_executable(test0 test0.C)
#add_executable(test1 test1.C)

target_link_libraries(YFitTests ${ROOT_LIBRARIES})


target_include_directories( YFitTests
    PUBLIC ${PROJECT_SOURCE_DIR}/YFit/include
    ${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS}
)
set_property(TARGET YFitTests PROPERTY CXX_STANDARD 17)

add_test(
  NAME test0
  COMMAND $<TARGET_FILE:test0>
  )

#ROOT_ADD_TEST(
#  NAME test1
#  COMMAND ${ROOT_root_CMD} -b -l -q $<TARGET_FILE:test1>
#  ENVIRONMENT ${ROOT_environ})