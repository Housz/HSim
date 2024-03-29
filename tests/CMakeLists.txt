add_executable(testvec2 test_vec2.cpp)
target_link_libraries(testvec2 PUBLIC HSim)

add_executable(testvec3 test_vec3.cpp)
target_link_libraries(testvec3 PUBLIC HSim)

add_executable(testvec4 test_vec4.cpp)
target_link_libraries(testvec4 PUBLIC HSim)

add_executable(testquat test_quaterion.cpp)
target_link_libraries(testquat PUBLIC HSim)

add_executable(testtrans test_transform3.cpp)
target_link_libraries(testtrans PUBLIC HSim)

add_executable(testvec test_vec.cpp)
target_link_libraries(testvec PUBLIC HSim)
target_link_libraries(testvec PUBLIC TBB::tbb)

add_executable(testparallel test_parallel.cpp)
target_link_libraries(testparallel PUBLIC HSim)
target_link_libraries(testparallel PUBLIC TBB::tbb)

add_executable(testmat test_mat.cpp)
target_link_libraries(testmat PUBLIC HSim)
target_link_libraries(testmat PUBLIC TBB::tbb)

add_executable(testmat33 test_mat33.cpp)
target_link_libraries(testmat33 PUBLIC HSim)

add_executable(testmat44 test_mat44.cpp)
target_link_libraries(testmat44 PUBLIC HSim)

add_executable(testscalarfield2 test_scalar_field2.cpp)
target_link_libraries(testscalarfield2 PUBLIC HSim)
target_link_libraries(testscalarfield2 PUBLIC TBB::tbb)

add_executable(testiovdb test_io_vdb.cpp)
target_link_libraries(testiovdb PUBLIC HSim)
target_link_libraries(testiovdb PUBLIC OpenVDB::openvdb)

add_executable(testscalargrid3 test_scalar_grid3.cpp)
target_link_libraries(testscalargrid3 PUBLIC HSim)
target_link_libraries(testscalargrid3 PUBLIC OpenVDB::openvdb)

add_executable(testfacecentergrid3 test_face_center_grid3.cpp)
target_link_libraries(testfacecentergrid3 PUBLIC HSim)
target_link_libraries(testfacecentergrid3 PUBLIC OpenVDB::openvdb)

add_executable(test_grid_fluid_solver3 test_grid_fluid_solver3.cpp)
target_link_libraries(test_grid_fluid_solver3 PUBLIC HSim)

add_executable(testsurface test_surface3.cpp)
target_link_libraries(testsurface PUBLIC HSim)

add_executable(testaabb3 test_aabb3.cpp)
target_link_libraries(testaabb3 PUBLIC HSim)

add_executable(testgl test_gl.cpp)
target_link_libraries(testgl PUBLIC glfw)
target_link_libraries(testgl PUBLIC glad::glad)
target_link_libraries(testgl PUBLIC imgui::imgui)
target_link_libraries(testgl PUBLIC HSim)

add_executable(testtinyobjloader test_tinyobjloader.cpp)
target_link_libraries(testtinyobjloader PUBLIC glfw)
target_link_libraries(testtinyobjloader PUBLIC glad::glad)
target_link_libraries(testtinyobjloader PUBLIC imgui::imgui)
target_link_libraries(testtinyobjloader PUBLIC tinyobjloader::tinyobjloader)
target_link_libraries(testtinyobjloader PUBLIC HSim)


add_executable(testtrimesh3 test_trianglemesh3.cpp)
target_link_libraries(testtrimesh3 PUBLIC glfw)
target_link_libraries(testtrimesh3 PUBLIC glad::glad)
target_link_libraries(testtrimesh3 PUBLIC tinyobjloader::tinyobjloader)
target_link_libraries(testtrimesh3 PUBLIC HSim)

add_executable(testbvh test_bvh3.cpp)
target_link_libraries(testbvh PUBLIC glfw)
target_link_libraries(testbvh PUBLIC glad::glad)
target_link_libraries(testbvh PUBLIC tinyobjloader::tinyobjloader)
target_link_libraries(testbvh PUBLIC HSim)


add_subdirectory(benchmarks)