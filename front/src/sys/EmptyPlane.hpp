// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_GLUE_SYS_EMPTYPLANE_HPP
#define MFPTLIB_GLUE_SYS_EMPTYPLANE_HPP

#include <pybind11/pybind11.h>


namespace mfptlib {

void def_empty_plane(pybind11::module& m);

} // namespace mfptlib

#endif
