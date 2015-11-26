include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/unit-tests)

# http://stackoverflow.com/a/21413672
macro (create_test target)
  add_test (NAME ${target} COMMAND $<TARGET_FILE:${target}>)
endmacro (create_test)

add_executable(bucket_test bucket.c)
target_link_libraries(bucket_test bucket)
create_test(bucket_test)

add_executable(map_test map.c)
target_link_libraries(map_test map)
create_test(map_test)

add_executable(vector_test vector.c)
target_link_libraries(vector_test vector)
create_test(vector_test)

add_executable(macro_table_test macro_table.c)
target_link_libraries(macro_table_test macro_table)
create_test(macro_table_test)