# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

project(mfptlib-back LANGUAGES CXX)


# ==== BACK-END LIBRARY ==== #

add_library(mfptlib-back STATIC)

target_include_directories(mfptlib-back PUBLIC
    include
)

target_link_libraries(mfptlib-back PUBLIC
    Eigen3::Eigen
    pcg-cpp
)

set_target_properties(mfptlib-back PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    POSITION_INDEPENDENT_CODE ON

    ${MFPTLIB_LTO_PROPS}
)

target_compile_options(mfptlib-back PRIVATE
    ${MFPTLIB_WARNING_FLAGS}
)

add_subdirectory(include)
add_subdirectory(src)


# ==== BACK-END UNIT TESTS ==== #

add_executable(mfptlib-back-test)

target_link_libraries(mfptlib-back-test PRIVATE
    Catch2::Catch2
    mfptlib-back
)

set_target_properties(mfptlib-back-test PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

target_compile_options(mfptlib-back-test PRIVATE
    ${MFPTLIB_WARNING_FLAGS}
)

add_subdirectory(test)
catch_discover_tests(mfptlib-back-test)
