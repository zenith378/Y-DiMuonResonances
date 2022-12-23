add_executable(test0 test_arg.C)
add_executable(test1 test_df_set.C)
add_executable(test2 test_fitRoo.C)
add_executable(test3 test_plot.C)
add_executable(test4 test_string.C)

target_link_libraries(test0  YFit)
target_link_libraries(test1 YFit ${ROOT_LIBRARIES})
target_link_libraries(test2 YFit ${ROOT_LIBRARIES})
target_link_libraries(test3 YFit ${ROOT_LIBRARIES})
target_link_libraries(test4 YFit ${ROOT_LIBRARIES})

set_property(TARGET test0 test1 test2 test3 test4 PROPERTY CXX_STANDARD 17)


add_test(
  NAME test0
  COMMAND $<TARGET_FILE:test0>
  )

add_test(
  NAME test1
  COMMAND $<TARGET_FILE:test1>
  )
set_tests_properties(test1 PROPERTIES TIMEOUT 5000)

add_test(
  NAME test2
  COMMAND $<TARGET_FILE:test2>
  )

add_test(
  NAME test3
  COMMAND $<TARGET_FILE:test3>
  )
add_test(
  NAME test4
  COMMAND $<TARGET_FILE:test4>
  )

#ROOT_ADD_TEST(
#  NAME test2
#  COMMAND ${ROOT_root_CMD} -b -l -q $<TARGET_FILE:test2>
#  ENVIRONMENT ${ROOT_environ})