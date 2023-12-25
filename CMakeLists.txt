cmake_minimum_required(VERSION 3.26)

project(HSim)

find_package(OpenVDB CONFIG REQUIRED)
find_package(TBB REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(benchmark REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/out/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/out/bin)

file(GLOB_RECURSE source CONFIGURE_DEPENDS include/*.h src/*.cpp)

add_library(HSim OBJECT ${source})
target_include_directories(HSim PUBLIC include)

add_subdirectory(examples)

add_subdirectory(tests)

# todo speed up compile