// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "Bath.hpp"

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/math/ExpMemoryBath.hpp>
#include <mfptlib/math/LangevinBath.hpp>

namespace py = pybind11;


namespace mfptlib {

void class_bath(pybind11::module& m)
{
    py::class_<Bath>{m, "Bath",
        "Type-erased wrapper representing noise and friction in the system."
    }
    .def("apply_forces",
        &Bath::apply_forces,
        "Apply noise and friction forces to the *momenta* of some states.",
        py::arg{"momenta"},
        py::arg{"masses"},
        py::arg{"dt"}
    )
    .def("filter_states",
        &Bath::filter_states,
        "Prepare internal state for the propagation of a sub-ensemble.",
        py::arg{"predicate"}
    )
    .def("reset",
        &Bath::reset,
        "Reset internal state in preparation for a different ensemble."
    );
}


void def_langevin_bath(pybind11::module& m)
{
    m.def("langevin_bath",
        [](double kb_t, double friction, Seed seed) -> Bath
            { return Bath{LangevinBath{kb_t, friction, seed}}; },
        R"----(
Return a Langevin bath with friction and memoryless noise.

:param kb_t: The temperature :math:`k_\mathrm{B} T`.
:param friction: The strength of the friction :math:`\gamma`.
:param seed: The seed used to initialize the PRNG.
        )----",
        py::arg{"kb_t"},
        py::arg{"friction"},
        py::arg{"seed"}
    );
}


void def_exp_memory_bath(pybind11::module& m)
{
    m.def("exp_memory_bath",
        [](double kb_t, double friction, double memory, Seed seed) -> Bath
            { return Bath{ExpMemoryBath{kb_t, friction, memory, seed}}; },
        R"----(
Return a bath with friction, noise, and an exponential memory kernel.

This bath requires :meth:`Bath.reset` to be called
before the bath can be reused for another ensemble.
The memory time scale is given by :math:`\tau = \gamma \alpha`.

:param kb_t: The temperature :math:`k_\mathrm{B} T`.
:param friction: The strength of the friction :math:`\gamma`.
:param memory: The memory parameter :math:`\alpha`.
:param seed: The seed used to initialize the PRNG.
        )----",
        py::arg{"kb_t"},
        py::arg{"friction"},
        py::arg{"memory"},
        py::arg{"seed"}
    );
}

} // namespace mfptlib
