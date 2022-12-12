// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "System.hpp"

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/sys/System.hpp>

namespace py = pybind11;


namespace mfptlib {

void class_system(pybind11::module& m)
{
    py::class_<System>{m, "System",
        "Type-erased wrapper representing the physical system."
    }
        .def("potential",
            [](const System& sys, const VectorsCRef& qp, double t) -> Scalars
                { return potential(sys, qp, t); },
            "Return the potential energy for states *qp* at time *t*.",
            py::arg{"qp"},
            py::arg{"t"}
        )
        .def("kinetic_energy",
            [](const System& sys, const VectorsCRef& qp) -> Scalars
                { return kinetic_energy(sys, qp); },
            "Return the kinetic energy for states *qp*.",
            py::arg{"qp"}
        )
        .def("total_energy",
            [](const System& sys, const VectorsCRef& qp, double t) -> Scalars
                { return total_energy(sys, qp, t); },
            "Return the total energy for states *qp* at time *t*.",
            py::arg{"qp"},
            py::arg{"t"}
        )
        .def("force",
            [](const System& sys, const VectorsCRef& qp, double t) -> Vectors
                { return force(sys, qp, t); },
            "Return the forces for states *qp* at time *t*.",
            py::arg{"qp"},
            py::arg{"t"}
        )
        .def("masses",
            [](const System& sys, const VectorsCRef& qp) -> Vectors
                { return masses(sys, qp); },
            "Return the masses for states *qp*.",
            py::arg{"qp"}
        )
        .def("dofs",
            [](const System& sys) -> Index
                { return degrees_of_freedom(sys); },
            "Return the system's number of degrees of freedom (DoFs)."
        );
}

} // namespace mfptlib
