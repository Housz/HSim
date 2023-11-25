find_package(TBB REQUIRED)

add_executable(testvec2 test_vec2.cpp)
add_executable(testvec3 test_vec3.cpp)
add_executable(testvec test_vec.cpp)
target_link_libraries(testvec PUBLIC TBB::tbb)


add_executable(testparallel test_parallel.cpp)
target_link_libraries(testparallel PUBLIC TBB::tbb)

