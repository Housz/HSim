cmake_minimum_required(VERSION 3.26)

add_executable(HSimulator main.cpp)
target_link_libraries(HSimulator PUBLIC glfw)
target_link_libraries(HSimulator PUBLIC glad::glad)
target_link_libraries(main PUBLIC glm::glm)

target_link_libraries(HSimulator PUBLIC imgui::imgui)

target_link_libraries(HSimulator PUBLIC HSim)

file(GLOB_RECURSE source CONFIGURE_DEPENDS GUI/*.h GUI/*.cpp)
add_library(HSimulator OBJECT ${source})
target_include_directories(HSimulator PUBLIC include)