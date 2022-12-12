# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

project(mfptlib-glue LANGUAGES CXX)


# ==== PYBIND11 MODULE ==== #

add_library(mfptlib-glue MODULE)

target_link_libraries(mfptlib-glue PRIVATE
    mfptlib-pybind11
    mfptlib-back
)

set_target_properties(mfptlib-glue PROPERTIES
    OUTPUT_NAME _backend
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"

    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON

    ${MFPTLIB_LTO_PROPS}
)

target_compile_options(mfptlib-glue PRIVATE
    ${MFPTLIB_WARNING_FLAGS}
)

add_subdirectory(src)

install(TARGETS mfptlib-glue LIBRARY DESTINATION front/py/mfptlib)

if(MFPTLIB_STRIP)
    if(CMAKE_STRIP)
        add_custom_command(
            TARGET mfptlib-glue
            POST_BUILD
            COMMAND ${CMAKE_STRIP} "$<TARGET_FILE:mfptlib-glue>"
        )
    else()
        message(WARNING "Failed to strip backend module: Executable not found.")
    endif()
endif()
