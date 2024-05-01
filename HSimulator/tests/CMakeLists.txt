cmake_minimum_required(VERSION 3.26)

# add_executable(test test.cpp)
# target_link_libraries(test PUBLIC HSimulatorLib)

add_executable(meshtest triangle_mesh3_test.cpp)
target_link_libraries(meshtest PUBLIC HSimulatorLib)

add_executable(rendertest render_test.cpp)
target_link_libraries(rendertest PUBLIC HSimulatorLib)


add_executable(gridtest grid_test.cpp)
target_link_libraries(gridtest PUBLIC HSimulatorLib)

