add_executable(bmsum bm_sum.cpp)
target_link_libraries(bmsum PUBLIC HSim)
target_link_libraries(bmsum PUBLIC TBB::tbb benchmark::benchmark)

add_executable(bmmat bm_mat.cpp)
target_link_libraries(bmmat PUBLIC HSim)
target_link_libraries(bmmat PUBLIC TBB::tbb benchmark::benchmark Eigen3::Eigen)

add_executable(bmgrid bm_grid.cpp)
target_link_libraries(bmgrid PUBLIC HSim)
target_link_libraries(bmgrid PUBLIC TBB::tbb benchmark::benchmark Eigen3::Eigen)
