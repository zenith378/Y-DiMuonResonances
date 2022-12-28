
add_executable(YLaunch main.C)

# Link the static library from subproject1
# This will cause the include directories for that target to be added to this project
target_link_libraries(YLaunch sub::lib1 ${ROOT_LIBRARIES})

set_property(TARGET YLaunch PROPERTY CXX_STANDARD 17)
