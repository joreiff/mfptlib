// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_GLUE_MATH_STEPPER_HPP
#define MFPTLIB_GLUE_MATH_STEPPER_HPP

#include <pybind11/pybind11.h>


namespace mfptlib {

void class_stepper(pybind11::module& m);
void def_baoab_stepper(pybind11::module& m);
void def_fast_baoab_stepper(pybind11::module& m);
void def_lf_middle_stepper(pybind11::module& m);

} // namespace mfptlib

#endif
