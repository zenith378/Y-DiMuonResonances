cmake_minimum_required (VERSION 3.5)

project(CMEPDACorCar VERSION 0.1)


# Add sub directories
add_subdirectory(main)
add_subdirectory(YFit)
#add_subdirectory(docs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest) # We include CTest which is part of CMake

add_subdirectory(test)

# We check if this is the main file
# you don't usually want users of your library to
# execute tests as part of their build
#if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
#    add_subdirectory(test)
#endif ()

enable_testing()