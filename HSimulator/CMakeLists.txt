cmake_minimum_required(VERSION 3.26)

add_executable(HSimulator)

target_link_libraries(HSimulator PUBLIC glfw)
target_link_libraries(HSimulator PUBLIC glad::glad)
target_link_libraries(HSimulator PUBLIC glm::glm)
target_link_libraries(HSimulator PUBLIC imgui::imgui)
# target_link_libraries(HSimulator PRIVATE Stb)

file(COPY resources DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Debug)
file(COPY resources DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/Release)

target_link_libraries(HSimulator PUBLIC HSim)

aux_source_directory(. sources)
aux_source_directory(GUI sources)
aux_source_directory(IO sources)
aux_source_directory(renderer sources)
aux_source_directory(scene sources)
aux_source_directory(config sources)

target_sources(HSimulator PUBLIC ${sources})

target_include_directories(HSimulator PUBLIC .)
