# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.15)

project(mfptlib LANGUAGES CXX)


# ==== CACHE VARIABLES ==== #

option(MFPTLIB_COLORED_DIAGNOSTICS "Force ANSI-colored compiler messages." OFF)
option(MFPTLIB_USE_LTO "Enable Linkt Time Optimization (LTO/IPO)." ON)
option(MFPTLIB_STRIP "Strip the resulting Python module." ON)

set(MFPTLIB_USE_LD "" CACHE STRING "Use a specific linker (e.g. lld, gold, bfd).")
set(MFPTLIB_USE_STDLIB "" CACHE STRING "Use a specific standard library (e.g. libc++).")


# ==== GLOBAL BUILD SETUP ==== #

if(NOT SKBUILD)
    message(WARNING "This package is meant to be installed using setup.py!")
endif()

include(cmake/Dependencies.cmake)
include(cmake/GlobalSetup.cmake)

mfptlib_select_build_type()

if(MFPTLIB_COLORED_DIAGNOSTICS)
    mfptlib_force_colored_diagnostics()
endif()

set(MFPTLIB_WARNING_FLAGS -Wall -Wextra -Wpedantic -Wconversion -Werror)
if(MFPTLIB_USE_LTO)
    set(MFPTLIB_LTO_PROPS
        INTERPROCEDURAL_OPTIMIZATION_RELEASE ON
        INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON
        INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON
    )
else()
    set(MFPTLIB_LTO_PROPS)
endif()

add_compile_options(
    # Optimize for current machine
    -march=native
    -mtune=native

    # Standard library settings
    "$<$<BOOL:${MFPTLIB_USE_STDLIB}>:-stdlib=${MFPTLIB_USE_STDLIB}>"
    # pybind11 requires sized delete in C++17 mode
    -fsized-deallocation
)
add_link_options(
    # Custom linker
    "$<$<BOOL:${MFPTLIB_USE_LD}>:-fuse-ld=${MFPTLIB_USE_LD}>"

    # Standard library settings
    "$<$<BOOL:${MFPTLIB_USE_STDLIB}>:-stdlib=${MFPTLIB_USE_STDLIB}>"
)


# ==== DEPENDENCIES ==== #

find_package(Python 3.6 REQUIRED COMPONENTS Interpreter Development)

mfptlib_fetch_dependency(
    TARGET Catch2
    GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
    GIT_TAG 62fd660583d3ae7a7886930b413c3c570e89786c # v2.13.9
    ADD_SUBDIR
)
mfptlib_fetch_dependency(
    TARGET Eigen3::Eigen
    GIT_REPOSITORY "https://gitlab.com/libeigen/eigen.git"
    GIT_TAG 3147391d946bb4b6c68edd901f2add6ac1f31f8c # v3.4.0
    HEADER_LIB "."
)
mfptlib_fetch_dependency(
    TARGET pcg-cpp
    GIT_REPOSITORY "https://github.com/imneme/pcg-cpp.git"
    GIT_TAG ffd522e7188bef30a00c74dc7eb9de5faff90092 # 2021-04-06
    HEADER_LIB "include"
)
mfptlib_fetch_dependency(
    TARGET pybind11
    GIT_REPOSITORY "https://github.com/pybind/pybind11.git"
    GIT_TAG 80dc998efced8ceb2be59756668a7e90e8bef917 # v2.10.1
    ADD_SUBDIR
)

add_library(mfptlib-pybind11 INTERFACE)
target_link_libraries(mfptlib-pybind11 INTERFACE Python::Python)
target_include_directories(mfptlib-pybind11 SYSTEM INTERFACE
    "${PYBIND11_INCLUDE_DIR}")


# ==== ADD DIRECTORIES ==== #

enable_testing()
include(${catch2_SOURCE_DIR}/contrib/Catch.cmake)

add_subdirectory(back)
add_subdirectory(front)
