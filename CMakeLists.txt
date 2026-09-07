cmake_minimum_required(VERSION 3.15)
project(tinyolfarve LANGUAGES CXX)

add_library(tinyolfarve STATIC
    src/color.cpp
)
add_library(tinyolfarve::tinyolfarve ALIAS tinyolfarve)

target_include_directories(tinyolfarve PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_compile_features(tinyolfarve PUBLIC cxx_std_14)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tinyolfarve PUBLIC -fno-exceptions -fno-rtti)
endif()
