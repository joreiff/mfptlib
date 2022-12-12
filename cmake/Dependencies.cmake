# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

include(FetchContent)


function(mfptlib_fetch_dependency)
    cmake_parse_arguments(ARG "ADD_SUBDIR" "TARGET;HEADER_LIB" "" ${ARGN})
    string(REGEX REPLACE "[:.+-]+" "_"  id "${ARG_TARGET}")
    string(TOLOWER "${id}" id)

    if(SKBUILD)
        set(src
            SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../deps/${id}"
            STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/../../deps/${id}.stamp"
        )
    else()
        set(src)
    endif()

    FetchContent_Declare(${id} ${src} ${ARG_UNPARSED_ARGUMENTS})
    FetchContent_GetProperties(${id})

    if(NOT ${id}_POPULATED)
        message(STATUS "Fetching ${ARG_TARGET}")
        FetchContent_Populate(${id})
        message(STATUS "Fetching ${ARG_TARGET} - done")

        if(ARG_ADD_SUBDIR)
            add_subdirectory(${${id}_SOURCE_DIR} ${${id}_BINARY_DIR})
        elseif(DEFINED ARG_HEADER_LIB)
            add_library(${ARG_TARGET} INTERFACE IMPORTED)
            target_include_directories(${ARG_TARGET} SYSTEM INTERFACE
                ${${id}_SOURCE_DIR}/${ARG_HEADER_LIB})
        endif()
    endif()

    set(${id}_SOURCE_DIR ${${id}_SOURCE_DIR} PARENT_SCOPE)
    set(${id}_BINARY_DIR ${${id}_BINARY_DIR} PARENT_SCOPE)
endfunction()
