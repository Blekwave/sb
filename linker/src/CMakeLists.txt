include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(bucket bucket.c)
add_library(vector vector.c)

add_library(map map.c)
target_link_libraries(map bucket)

add_library(hash_aux hash_aux.c)
add_library(str_aux str_aux.c)

# add_executable(expansor main.c)
# target_link_libraries(expansor expander)

