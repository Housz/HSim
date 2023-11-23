cmake_minimum_required(VERSION 3.20)

project(HSim)

include_directories(include)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(main)
target_sources(main PUBLIC ${sources})

add_subdirectory(tests)

# file(GLOB_RECURSE tests_srcs CONFIGURE_DEPENDS tests/*.cpp)
# add_executable(test)
# target_sources(test PUBLIC ${tests_srcs})

