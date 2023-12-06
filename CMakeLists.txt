cmake_minimum_required(VERSION 3.20)

project(HSim)

include_directories(include)

# file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/.cpp)
# add_executable(main)
# target_sources(main PUBLIC ${sources})

add_executable(main src/main.cpp)
add_executable(my_scalar_field2 src/my_scalar_field2.cpp)
add_executable(my_scalar_field3 src/my_scalar_field3.cpp)

add_subdirectory(tests)

# file(GLOB_RECURSE tests_srcs CONFIGURE_DEPENDS tests/*.cpp)
# add_executable(test)
# target_sources(test PUBLIC ${tests_srcs})

