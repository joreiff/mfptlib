// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_GLUE_MATH_BATH_HPP
#define MFPTLIB_GLUE_MATH_BATH_HPP

#include <pybind11/pybind11.h>


namespace mfptlib {

void class_bath(pybind11::module& m);
void def_langevin_bath(pybind11::module& m);
void def_exp_memory_bath(pybind11::module& m);

} // namespace mfptlib

#endif
