cmake_minimum_required(VERSION 3.28.1 FATAL_ERROR)

project(SMG_REMAKE)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Wvla -pedantic)
add_link_options(-g -O0)

include(cgp/library/CMakeLists.txt)

add_executable(main)

add_library(cgp SHARED)

target_include_directories(cgp PRIVATE
    ${CMAKE_SOURCE_DIR}/cgp/library/
)

target_sources(cgp PRIVATE
    ${src_files_cgp})

target_link_libraries(main PRIVATE
    cgp
)

target_sources(main PRIVATE
    src/main.cc
)
