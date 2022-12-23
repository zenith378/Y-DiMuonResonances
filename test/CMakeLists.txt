add_executable(test0 test_arg.C)
add_executable(test1 test_df_set.C)
add_executable(test2 test_fitRoo.C)

target_link_libraries(test0 YFit)
target_link_libraries(test1 YFit ${ROOT_LIBRARIES})
target_link_libraries(test2 YFit ${ROOT_LIBRARIES})

set_property(TARGET test0 PROPERTY CXX_STANDARD 17)
set_property(TARGET test1 PROPERTY CXX_STANDARD 17)
set_property(TARGET test2 PROPERTY CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 17)

add_test(
  NAME test0
  COMMAND $<TARGET_FILE:test0>
  )

#add_test(
#  NAME test1
#  COMMAND $<TARGET_FILE:test1>
#  )

add_test(
  NAME test2
  COMMAND $<TARGET_FILE:test2>
  )
#set_tests_properties(test1 PROPERTIES TIMEOUT 5000)

#ROOT_ADD_TEST(
#  NAME test2
#  COMMAND ${ROOT_root_CMD} -b -l -q $<TARGET_FILE:test2>
#  ENVIRONMENT ${ROOT_environ})