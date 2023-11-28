find_package(TBB REQUIRED)
find_package(benchmark REQUIRED)

add_executable(bmsum bm_sum.cpp)
target_link_libraries(bmsum PUBLIC TBB::tbb)
target_link_libraries(bmsum PUBLIC benchmark::benchmark)