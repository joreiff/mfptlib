// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include "LithiumCyanide.hpp"

#include <pybind11/eigen.h>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/sys/LithiumCyanide.hpp>
#include <mfptlib/sys/System.hpp>

namespace py = pybind11;


namespace mfptlib {

void def_lithium_cyanide(pybind11::module& m)
{
    m.def("lithium_cyanide",
        []() -> System { return System{LithiumCyanide{}}; },
        R"----(
Return a system modeling the LiNC ⇋ LiCN isomerization reaction.

The implementation is based on [Esser1982]_ with corrections from [Schle2022]_.
        )----"
    );

}

} // namespace mfptlib
