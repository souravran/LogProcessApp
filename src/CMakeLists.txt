cmake_minimum_required(VERSION 2.8)
project(LogProcessor-Application)

include(cmake/c++11.cmake)
#if doxygen documentation not required, then comment out the line below 
include(cmake/doxygen.cmake)

INCLUDE(FindPkgConfig)
#no external package required for building, package checking can be done using: pkg_check_modules(), find_package()

include_directories(
    ${PROJECT_SOURCE_DIR}
)

link_directories(
    #nothing to link against, just a place holder
)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/data.log")
     configure_file(${CMAKE_SOURCE_DIR}/data.log ${CMAKE_BINARY_DIR}/data.log)
endif()

add_subdirectory(application)
