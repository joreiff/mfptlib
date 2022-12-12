// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "Stepper.hpp"

#include <utility>

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/BaoabStepper.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/math/FastBaoabStepper.hpp>
#include <mfptlib/math/LfMiddleStepper.hpp>
#include <mfptlib/math/Stepper.hpp>
#include <mfptlib/sys/System.hpp>

namespace py = pybind11;


namespace mfptlib {

void class_stepper(pybind11::module& m)
{
    py::class_<Stepper>{m, "Stepper",
        "Type-erased wrapper representing the integration scheme."
    }
    .def("step",
        &Stepper::step,
        py::call_guard<py::gil_scoped_release>{},
        "Propagate states *qp* of *system* at time *t* by a single time step.",
        py::arg{"bath"},
        py::arg{"system"},
        py::arg{"qp"},
        py::arg{"t"}
    )
    .def("filter_states",
        &Stepper::filter_states,
        "Prepare internal state for the propagation of a sub-ensemble."
    )
    .def("reset",
        &Stepper::reset,
        "Reset internal state in preparation for a different ensemble."
    );
}


void def_baoab_stepper(py::module& m)
{
    m.def("baoab_stepper",
        [](double dt) -> Stepper { return Stepper{BaoabStepper{dt}}; },
        R"----(
Return a stepper implementing the BAOAB integrator scheme.

This integrator has very small configurational sampling errors.
Certain statistical properties in a harmonic potential
can even be reproduced exactly,
independent of the time step.
Kinetic properties, however, exhibit much larger errors.

See [Leimk2015]_ and [Fass2018]_ for details about the BAOAB scheme.

:param dt: The integration step size :math:`\Delta t`.
        )----",
        py::arg{"dt"}
    );
}


void def_fast_baoab_stepper(py::module& m)
{
    m.def("fast_baoab_stepper",
        [](double dt) -> Stepper { return Stepper{FastBaoabStepper{dt}}; },
        R"----(
Return a stepper implementing the BAOAB integrator scheme with force caching.

This stepper requires :meth:`Stepper.reset` to be called
before the stepper can be reused for another ensemble.
See :func:`baoab_stepper` for more details.

:param dt: The integration step size :math:`\Delta t`.
        )----",
        py::arg{"dt"}
    );
}


void def_lf_middle_stepper(py::module& m)
{
    m.def("lf_middle_stepper",
        [](double dt) -> Stepper { return Stepper{LfMiddleStepper{dt}}; },
        R"----(
Return a stepper implementing the LF-Middle integrator scheme.

Note that this is a leapfrog version of the BAOAB scheme,
meaning that positions always leap ahead by half a time step.
As a result,
examining the phase space configuration of states requires extra case.
In return, however,
a more accurate sampling of the thermal ensemble can be achieved.

See [Zhang2019]_ for details about the LF-Middle scheme.

:param dt: The integration step size :math:`\Delta t`.
        )----",
        py::arg{"dt"}
    );
}

} // namespace mfptlib
