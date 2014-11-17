cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(xmradio)

option(XMR_BUILD_TESTS "Build the tests" ON)

if (XMR_BUILD_TESTS)
	enable_testing()
endif()

set(version 1.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml REQUIRED)

if (XMR_BUILD_TESTS)
	find_package(Qt5Test REQUIRED)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(cmake/macro.cmake)

# uninstall target
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)

add_custom_target(uninstall
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

include(CheckIncludeFiles)
include(FindPkgConfig)

set(package xmradio)
set(prefix	${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(bindir ${exec_prefix}/bin)
set(libdir ${exec_prefix}/lib)

set(radiodir ${libdir}/${package}/radio)

set(XMR_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/xmradio)
set(EXECUTABLE_OUTPUT_PATH ${XMR_OUTPUT_PATH})
set(LIBRARY_OUTPUT_PATH ${XMR_OUTPUT_PATH})

include_directories(${PROJECT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)
