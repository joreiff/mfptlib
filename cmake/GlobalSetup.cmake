# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

function(mfptlib_select_build_type)
    if(CMAKE_CONFIGURATION_TYPES)
        return()
    endif()

    set(available Debug Release MinSizeRel RelWithDebInfo)
    set(default Release)

    if(CMAKE_BUILD_TYPE)
        string(TOLOWER "${CMAKE_BUILD_TYPE}" selected_lower)
        string(TOLOWER "${available}" available_lower)
        if(NOT selected_lower IN_LIST available_lower)
            message(FATAL_ERROR "CMAKE_BUILD_TYPE must be one of ${available}")
        endif()
    else()
        message(STATUS
            "Setting build type to '${default}' as none was specified.")
        set(CMAKE_BUILD_TYPE ${default} CACHE STRING
            "The type of build (Release, Debug, ...)." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${available})
    endif()
endfunction()


function(mfptlib_force_colored_diagnostics)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    else()
        message(WARNING "Unknown compiler, cannot colorize diagnostics.")
    endif()
endfunction()
