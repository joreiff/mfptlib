// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "Observer.hpp"

#include <pybind11/eigen.h>
#include <pybind11/functional.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Observer.hpp>

namespace py = pybind11;


namespace mfptlib {

void class_observer(pybind11::module& m)
{
    py::class_<Observer>{m, "Observer",
        "Type-erased function used to observe the state during propagation."
    }
    .def(py::init<Observer::Function>(),
        "Construct an Observer from a Python function. Defaults to a no-op.",
        py::arg{"func"} = Observer::Function{}
    )
    .def("__call__",
        &Observer::operator(),
        "Call the observer function for states *qp* at time *t*.",
        py::arg{"qp"},
        py::arg{"t"}
    );
}

} // namespace mfptlib
