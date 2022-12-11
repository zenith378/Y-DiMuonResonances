cmake_minimum_required (VERSION 3.5)

project(CMEPDACorCar VERSION 0.1)

# Add sub directories
add_subdirectory(main)
add_subdirectory(YFit)
add_subdirectory(docs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
