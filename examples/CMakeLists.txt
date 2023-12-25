cmake_minimum_required(VERSION 3.26)


add_executable(main main.cpp)
target_link_libraries(main PUBLIC HSim)

add_executable(my_scalar_field2 my_scalar_field2.cpp)
target_link_libraries(my_scalar_field2 PUBLIC HSim)

add_executable(my_scalar_field3 my_scalar_field3.cpp)
target_link_libraries(my_scalar_field3 PUBLIC HSim)
target_link_libraries(my_scalar_field3 PUBLIC OpenVDB::openvdb)

add_executable(single_phase_grid_fluid single_phase_grid_fluid.cpp)
