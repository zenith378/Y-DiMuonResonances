# Doxygen

# look for Doxygen package
find_package(Doxygen)

if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/build/docs/Doxyfile.out)
    set(DOXYGEN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/build/docs/doxygen)
    #set(DOXIGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/YFit)
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # Note: do not put "ALL" - this builds docs together with application EVERY TIME!
    add_custom_target( docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

find_package(Sphinx REQUIRED)

set(SPHINX_SOURCE ${PROJECT_SOURCE_DIR}/docs)
set(SPHINX_BUILD ${PROJECT_SOURCE_DIR}/build/docs/sphinx)

add_custom_target(Sphinx ALL
                  COMMAND
                  ${SPHINX_EXECUTABLE} -b html
                  ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build
                  COMMENT "Generating documentation with Sphinx")