include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(bucket bucket.c)
add_library(map map.c)
add_library(vector vector.c)

add_executable(mock mock.c)
