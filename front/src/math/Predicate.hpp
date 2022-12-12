// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_GLUE_MATH_PREDICATE_HPP
#define MFPTLIB_GLUE_MATH_PREDICATE_HPP

#include <pybind11/pybind11.h>


namespace mfptlib {

void class_predicate(pybind11::module& m);

} // namespace mfptlib

#endif
