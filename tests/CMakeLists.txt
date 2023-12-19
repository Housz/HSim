cmake_minimum_required(VERSION 3.26)
find_package(OpenVDB CONFIG REQUIRED)
find_package(TBB REQUIRED)

add_executable(testvec2 test_vec2.cpp)
add_executable(testvec3 test_vec3.cpp)
add_executable(testvec test_vec.cpp)
target_link_libraries(testvec PUBLIC TBB::tbb)

add_executable(testparallel test_parallel.cpp)
target_link_libraries(testparallel PUBLIC TBB::tbb)

add_executable(testmat test_mat.cpp)
target_link_libraries(testmat PUBLIC TBB::tbb)

add_executable(testscalarfield2 test_scalar_field2.cpp)
target_link_libraries(testscalarfield2 PUBLIC TBB::tbb)

add_executable(testiovdb test_io_vdb.cpp)
target_link_libraries(testiovdb PUBLIC OpenVDB::openvdb)

add_executable(testscalargrid3 test_scalar_grid3.cpp)
target_link_libraries(testscalargrid3 PUBLIC OpenVDB::openvdb)

add_executable(testfacecentergrid3 test_face_center_grid3.cpp)
target_link_libraries(testfacecentergrid3 PUBLIC OpenVDB::openvdb)

add_subdirectory(benchmarks)





