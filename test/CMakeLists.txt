# SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
#
# SPDX-License-Identifier: MIT

include(FetchContent)
FetchContent_Declare(unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.7.0
    GIT_SHALLOW TRUE
    SYSTEM
    EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(unity)

add_executable(tinyolfarve_tests test_color/test_color.cpp)
target_link_libraries(tinyolfarve_tests PRIVATE tinyolfarve::tinyolfarve unity::framework)
set_target_properties(tinyolfarve_tests PROPERTIES CXX_EXTENSIONS OFF)

add_test(NAME tinyolfarve_tests COMMAND tinyolfarve_tests)
