cmake_minimum_required(VERSION 3.4)

# A Quite OK Image format implementation written in C++
project(qoi
    VERSION 0.1
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

include(${PROJECT_SOURCE_DIR}/cmake/load_config.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/load_tools.cmake)
add_compile_commands_json()

include(${PROJECT_SOURCE_DIR}/cmake/flags.cmake)

add_library(qoi STATIC SHARED src/qoi.cpp include/qoi.hpp)
add_library(qoi::qoi ALIAS qoi)
target_include_directories(qoi
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
