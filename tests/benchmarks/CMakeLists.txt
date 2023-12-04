find_package(TBB REQUIRED)
find_package(benchmark REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(bmsum bm_sum.cpp)
target_link_libraries(bmsum PUBLIC TBB::tbb benchmark::benchmark)

add_executable(bmmat bm_mat.cpp)
target_link_libraries(bmmat PUBLIC TBB::tbb benchmark::benchmark Eigen3::Eigen)
