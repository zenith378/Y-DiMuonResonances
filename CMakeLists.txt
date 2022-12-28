cmake_minimum_required (VERSION 3.5)

project(CMEPDACorCar VERSION 1.0)


# Add sub directories
add_subdirectory(main)
add_subdirectory(YFit)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest) # We include CTest which is part of CMake

add_subdirectory(test)

enable_testing()
