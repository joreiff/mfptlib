// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_SYS_HARMONICOSCILLATOR_HPP
#define MFPTLIB_SYS_HARMONICOSCILLATOR_HPP

#include <cassert>
#include <stdexcept>
#include <utility>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

class HarmonicOscillator
{
public:
    explicit HarmonicOscillator(Vector masses, Vector strengths) noexcept
        : masses_{std::move(masses)}
        , strengths_{std::move(strengths)}
    {
        expect(masses.size() == strengths.size(),
            "The number of masses must match the number of strengths.");
    }


    auto masses() const noexcept -> const Vector&
    { return masses_; }

    auto strengths() const noexcept -> const Vector&
    { return strengths_; }


private:
    Vector masses_;
    Vector strengths_;
};


[[nodiscard]]
inline auto degrees_of_freedom(
    const HarmonicOscillator& model
) noexcept -> Index
{ return model.masses().size(); }


[[nodiscard]]
inline auto potential(
    const HarmonicOscillator& model,
    const VectorsCRef& states,
    [[maybe_unused]] double t
) noexcept -> Scalars
{
    assert(states.cols() == 2 * degrees_of_freedom(model));
    return 0.5 * (
        positions(states).square().rowwise() * model.strengths()
    ).rowwise().sum();
}


[[nodiscard]]
inline auto force(
    const HarmonicOscillator& model,
    const VectorsCRef& states,
    [[maybe_unused]] double t
) noexcept -> Vectors
{
    assert(states.cols() == 2 * degrees_of_freedom(model));
    return positions(states).rowwise() * -model.strengths();
}


[[nodiscard]]
inline auto masses(
    const HarmonicOscillator& model,
    const VectorsCRef& states
) noexcept -> Vectors
{
    assert(states.cols() == 2 * degrees_of_freedom(model));
    return model.masses().replicate(states.rows(), 1);
}

} // namespace mfptlib


#endif
