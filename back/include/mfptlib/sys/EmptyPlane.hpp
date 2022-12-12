// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_SYS_EMPTYPLANE_HPP
#define MFPTLIB_SYS_EMPTYPLANE_HPP

#include <cassert>

#include <mfptlib/core/Types.hpp>


namespace mfptlib {

struct EmptyPlane
{
    Vector masses;
};


[[nodiscard]]
inline auto degrees_of_freedom(const EmptyPlane& model) noexcept -> Index
{ return model.masses.size(); }


[[nodiscard]]
inline auto potential(
    [[maybe_unused]] const EmptyPlane& model,
    const VectorsCRef& states,
    [[maybe_unused]] double t
) noexcept -> Scalars
{
    assert(states.cols() == 2 * degrees_of_freedom(model));
    return Scalars::Zero(states.rows());
}


[[nodiscard]]
inline auto force(
    [[maybe_unused]] const EmptyPlane& model,
    const VectorsCRef& states,
    [[maybe_unused]] double t
) noexcept -> Vectors
{
    assert(states.cols() == 2 * degrees_of_freedom(model));
    return Vectors::Zero(states.rows(), model.masses.size());
}


[[nodiscard]]
inline auto masses(
    const EmptyPlane& model,
    const VectorsCRef& states
) noexcept -> Vectors
{
    assert(states.cols() == 2 * degrees_of_freedom(model));
    return model.masses.replicate(states.rows(), 1);
}

} // namespace mfptlib


#endif
