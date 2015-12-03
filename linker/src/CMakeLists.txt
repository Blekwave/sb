include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(bucket bucket.c)
add_library(vector vector.c)

add_library(map map.c)
target_link_libraries(map bucket)

add_library(hash_aux hash_aux.c)
add_library(str_aux str_aux.c)

add_library(sym_table sym_table.c)
target_link_libraries(sym_table map hash_aux)

add_library(linker linker.c)
target_link_libraries(linker sym_table vector str_aux)

add_executable(ligador main.c)
target_link_libraries(ligador linker str_aux vector)

