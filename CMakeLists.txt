cmake_minimum_required(VERSION 3.5.0)
project(kmeans VERSION 0.1.0 LANGUAGES C CXX)

include_directories( include)


# Add executable targets
add_executable(s_AoS kmeans_AoS.cpp)
add_executable(s_SoA kmeans_SoA.cpp)
add_executable(sequentialSolver sequential_SoA.cpp)



find_package(OpenMP REQUIRED)
add_executable(solver parallel_kmeans.cpp)

target_link_libraries(solver PRIVATE OpenMP::OpenMP_CXX)

add_custom_command(TARGET s_AoS s_SoA POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/points.csv
        ${CMAKE_BINARY_DIR}/points.csv)

add_custom_command(TARGET s_AoS s_SoA POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/data.csv
        ${CMAKE_BINARY_DIR}/data.csv)

add_custom_command(TARGET s_AoS s_SoA POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/data/output.csv
        ${CMAKE_BINARY_DIR}/output.csv)

