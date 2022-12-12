// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_GLUE_MATH_PROPAGATE_HPP
#define MFPTLIB_GLUE_MATH_PROPAGATE_HPP

#include <pybind11/pybind11.h>


namespace mfptlib {

void def_propagate_to(pybind11::module& m);
void def_propagate_while(pybind11::module& m);

} // namespace mfptlib

#endif
