cmake_minimum_required(VERSION 3.20)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(HSim)

find_package(OpenVDB CONFIG REQUIRED)
find_package(TBB REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(benchmark REQUIRED)
find_package(tinyobjloader REQUIRED)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# if (NOT CMAKE_BUILD_TYPE)
# 	set (CMAKE_BUILD_TYPE Debug)
# endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/out/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/out/bin)

file(GLOB_RECURSE source CONFIGURE_DEPENDS include/*.h src/*.cpp)
# file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)


add_library(HSim OBJECT ${source})
target_include_directories(HSim PUBLIC include)

target_link_libraries(HSim PUBLIC glfw)
target_link_libraries(HSim PUBLIC glad::glad)
target_link_libraries(HSim PUBLIC glm::glm)


option(BUILD_HSIMULATOR "build tests" OFF)
if(BUILD_HSIMULATOR)
	add_subdirectory(HSimulator)
endif()

option(BUILD_EXAMPLES "build tests" OFF)
if(BUILD_TESTS)
	add_subdirectory(examples)
endif()

option(BUILD_TESTS "build tests" OFF)
if(BUILD_TESTS)
	add_subdirectory(tests)
endif()



##############################################################################
if (WIN32)
	add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

if (NOT MSVC)
	find_program(CCACHE_PROGRAM ccache)
	if (CCACHE PROGRAM)
		message(STATUS "Found CCache: sCCACHE_PROGRAM]")
		set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
		set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PROGRAM})
	endif()
endif()