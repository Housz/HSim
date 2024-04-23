cmake_minimum_required(VERSION 3.26)

# add_executable(test test.cpp)
# target_link_libraries(test PUBLIC HSimulatorLib)

# add_executable(testtrimesh33 triangle_mesh3_test.cpp)
# target_link_libraries(testtrimesh33 PUBLIC HSimulatorLib)

add_executable(rendertest render_test.cpp)
# target_link_libraries(rendertest PUBLIC HSim)
target_link_libraries(rendertest PUBLIC HSimulatorLib)

