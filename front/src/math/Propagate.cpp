// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "Propagate.hpp"
#include "mfptlib/math/Propagate.hpp"

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Observer.hpp>
#include <mfptlib/math/Predicate.hpp>
#include <mfptlib/math/Stepper.hpp>
#include <mfptlib/sys/System.hpp>

namespace py = pybind11;


namespace mfptlib {

void def_propagate_to(pybind11::module& m)
{
    m.def("propagate_to",
        &propagate_to,
        py::call_guard<py::gil_scoped_release>{},
        R"----(
Propagate states *qp* of *system* from time *t* to *t_end*.

:param stepper: The implementation of the integrator scheme.
:param bath: The implementation of noise and friction from the surrounding bath.
:param system: The physical system to propagate.
:param qp: The states at initial time *t*.
:param t: The initial time.
:param t_end: The target for the final time.
:param observer: A callback being called before/after every integrator step.
:returns: The actual final time ≥ *t_end*.
    It may differ from *t_end* because of finite integrator steps.
        )----",
        py::arg{"stepper"},
        py::arg{"bath"},
        py::arg{"system"},
        py::arg{"qp"},
        py::arg{"t"},
        py::arg{"t_end"},
        py::arg{"observer"} = Observer{}
    );
}


void def_propagate_while(pybind11::module& m)
{
    m.def("propagate_while",
        &propagate_while,
        py::call_guard<py::gil_scoped_release>{},
        R"----(
Propagate states *qp* of *system* from time *t* while *predicate* holds true.

The *predicate* and the *observer* are only called with
the subset of states that are still actively being propagated.

:param stepper: The implementation of the integrator scheme.
:param bath: The implementation of noise and friction from the surrounding bath.
:param system: The physical system to propagate.
:param qp: The states at initial time *t*.
:param t: The initial time.
:param predicate: A function that determines
    which states should continue to propagate.
:param observer: A callback being called before/after every integrator step
    with the actively propagating states.
:returns: The final times of the states being propagated.
        )----",
        py::arg{"stepper"},
        py::arg{"bath"},
        py::arg{"system"},
        py::arg{"qp"},
        py::arg{"t"},
        py::arg{"predicate"},
        py::arg{"observer"} = Observer{}
    );
}

} // namespace mfptlib
