# SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.28...4.2)
project(tinyolfarve LANGUAGES CXX)

include(GNUInstallDirs)

add_library(tinyolfarve STATIC)
add_library(tinyolfarve::tinyolfarve ALIAS tinyolfarve)

target_sources(tinyolfarve
    PRIVATE
        src/color.cpp
        src/tinyolfarve/cie.hpp
        src/tinyolfarve/model.hpp
        src/tinyolfarve/platform.hpp
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS src
            FILES
                src/tinyolfarve/color.hpp
                src/tinyolfarve/tinyolfarve.hpp
)

target_compile_features(tinyolfarve PUBLIC cxx_std_11)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tinyolfarve PRIVATE -fno-exceptions -fno-rtti)
endif()

if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
    if(BUILD_TESTING)
        add_subdirectory(test)
    endif()
endif()

install(TARGETS tinyolfarve
    FILE_SET HEADERS
)
