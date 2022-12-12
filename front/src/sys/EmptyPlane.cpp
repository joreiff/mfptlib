// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "EmptyPlane.hpp"

#include <utility>

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/sys/EmptyPlane.hpp>
#include <mfptlib/sys/System.hpp>

namespace py = pybind11;


namespace mfptlib {

void def_empty_plane(pybind11::module& m)
{
    m.def("empty_plane",
        [](Vector masses) -> System
            { return System{EmptyPlane{std::move(masses)}}; },
        R"----(
Return a test system with :math:`n` degrees of freedom and potential :math:`V = 0`.

:param masses: The :math:`n` masses corresponding to each dimension.
        )----",
        py::arg{"masses"}
    );

}

} // namespace mfptlib
