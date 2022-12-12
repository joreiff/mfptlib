// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "HarmonicOscillator.hpp"

#include <utility>

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/sys/HarmonicOscillator.hpp>
#include <mfptlib/sys/System.hpp>

namespace py = pybind11;


namespace mfptlib {

void def_harmonic_oscillator(pybind11::module& m)
{
    m.def("harmonic_oscillator",
        [](Vector masses, Vector strengths) -> System
        {
            return System{HarmonicOscillator{
                std::move(masses), std::move(strengths)}};
        },
        R"----(
Return a test system with a potential given by :math:`n` harmonic oscillators.

.. math::

    V(\vec{q}) = \frac{1}{2} \sum_{j = 1}^{n} A_j q_j^2

:param masses: The :math:`n` masses corresponding to each dimension.
:param strengths: The :math:`n` strengths :math:`\vec{A}` of the oscillators.
        )----",
        py::arg{"masses"},
        py::arg{"strengths"}
    );

}

} // namespace mfptlib
