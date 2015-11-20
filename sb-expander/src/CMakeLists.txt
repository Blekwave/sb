include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(bucket bucket.c)

add_library(map map.c)
target_link_libraries(map bucket)

add_library(vector vector.c)

add_library(hash_aux hash_aux.c)

add_library(line_parser line_parser.c)

add_library(str_aux str_aux.c)

add_library(macro_table macro_table.c)
target_link_libraries(macro_table hash_aux line_parser map vector str_aux)

add_executable(mock mock.c)
