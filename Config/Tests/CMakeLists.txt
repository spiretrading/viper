file(GLOB_RECURSE source_files ${VIPER_SOURCE_PATH}/Tests/*.cpp)
add_executable(viper_tester ${source_files})
source_group(TREE ${VIPER_SOURCE_PATH}/Tests PREFIX "Source Files"
  FILES ${source_files})
target_include_directories(viper_tester SYSTEM PRIVATE
  ${DOCTEST_INCLUDE_PATH} ${MYSQL_INCLUDE_PATH} ${SQLITE_INCLUDE_PATH})
target_link_libraries(viper_tester
  debug ${SQLITE_LIBRARY_DEBUG_PATH}
  optimized ${SQLITE_LIBRARY_OPTIMIZED_PATH})
if(UNIX)
  target_link_libraries(viper_tester dl pthread)
endif()
add_custom_command(TARGET viper_tester POST_BUILD COMMAND viper_tester)
install(TARGETS viper_tester CONFIGURATIONS Debug
  DESTINATION ${TEST_INSTALL_DIRECTORY}/Debug)
install(TARGETS viper_tester CONFIGURATIONS Release RelWithDebInfo
  DESTINATION ${TEST_INSTALL_DIRECTORY}/Release)
