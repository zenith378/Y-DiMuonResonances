add_executable(test0 test_arg.C)
add_executable(test1 test_df_set.C)
add_executable(test2 test_fitRoo.C)
add_executable(test3 test_plot.C)
add_executable(test4 test_string.C)

target_link_libraries(test0  YCrossFit)
target_link_libraries(test1 YCrossFit ${ROOT_LIBRARIES})
target_link_libraries(test2 YCrossFit ${ROOT_LIBRARIES})
target_link_libraries(test3 YCrossFit ${ROOT_LIBRARIES})
target_link_libraries(test4 YCrossFit ${ROOT_LIBRARIES})

set_property(TARGET test0 test1 test2 test3 test4 PROPERTY CXX_STANDARD 17)


add_test(
  NAME CmdLineArgs
  COMMAND $<TARGET_FILE:test0>
  )

add_test(
  NAME ReadAndStoreData
  COMMAND $<TARGET_FILE:test1>
  )
set_tests_properties(ReadAndStoreData PROPERTIES TIMEOUT 8000)

add_test(
  NAME Fit
  COMMAND $<TARGET_FILE:test2>
  )

add_test(
  NAME SavePlot
  COMMAND $<TARGET_FILE:test3>
  )

add_test(
  NAME StringFormat
  COMMAND $<TARGET_FILE:test4>
  )

#ROOT_ADD_TEST(
#  NAME test2
#  COMMAND ${ROOT_root_CMD} -b -l -q $<TARGET_FILE:test2>
#  ENVIRONMENT ${ROOT_environ})