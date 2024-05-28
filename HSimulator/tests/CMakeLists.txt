cmake_minimum_required(VERSION 3.26)

# add_executable(test test.cpp)
# target_link_libraries(test PUBLIC HSimulatorLib)

add_executable(meshTest triangle_mesh3_test.cpp)
target_link_libraries(meshTest PUBLIC HSimulatorLib)

add_executable(renderTest render_test.cpp)
target_link_libraries(renderTest PUBLIC HSimulatorLib)


add_executable(gridTest grid_test.cpp)
target_link_libraries(gridTest PUBLIC HSimulatorLib)


add_executable(naiveSolverTest naive_solver_test.cpp)
target_link_libraries(naiveSolverTest PUBLIC HSimulatorLib)

add_executable(naiveFluidSolverTest naive_fluid_solver_test.cpp)
target_link_libraries(naiveFluidSolverTest PUBLIC HSimulatorLib)

add_executable(naiveSmokeSolverTest naive_smoke_solver_test.cpp)
target_link_libraries(naiveSmokeSolverTest PUBLIC HSimulatorLib)


add_executable(smokeSolverTest smoke_solver_test.cpp)
target_link_libraries(smokeSolverTest PUBLIC HSimulatorLib)

add_executable(linearSystemTest linearsystem_test.cpp)
target_link_libraries(linearSystemTest PUBLIC HSimulatorLib)

