// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "Predicate.hpp"

#include <pybind11/eigen.h>
#include <pybind11/functional.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Predicate.hpp>

namespace py = pybind11;


namespace mfptlib {

void class_predicate(pybind11::module& m)
{
    py::class_<Predicate>{m, "Predicate",
        "Type-erased function determining whether propagation should continue."
    }
    .def(py::init<Predicate::Function>(),
        "Construct a Predicate from a Python function.",
        py::arg{"func"}
    )
    .def("__call__",
        &Predicate::operator(),
        "Evaluate the predicate function for states *qp* at time *t*.",
        py::arg{"qp"},
        py::arg{"t"}
    );
}

} // namespace mfptlib
