// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_TEST_EULERSTEPPER_HPP
#define MFPTLIB_TEST_EULERSTEPPER_HPP

#include <cstddef>
#include <memory>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/math/Stepper.hpp>
#include <mfptlib/sys/System.hpp>


namespace mfptlib::test {

struct EulerStepper
{
    struct Stats
    {
        std::size_t step{};
        std::size_t filter_states{};
        std::size_t reset{};
    };
    using StatsPtr = std::shared_ptr<Stats>;

    double dt;
    StatsPtr stats;

    void step(Bath& bath, const System& system, VectorsRef states, double& t)
    {
        const Vectors f = force(system, states, t);
        const Vectors m = masses(system, states);
        positions(states) += dt * momenta(states) / m;
        momenta(states) += dt * f;
        bath.apply_forces(momenta(states), m, dt);
        t += dt;
        ++stats->step;
    }

    void filter_states(const Booleans&) noexcept
    { ++stats->filter_states; }

    void reset() noexcept
    { ++stats->reset; }
};


inline auto euler_stepper(double dt = 1.0) noexcept
    -> std::pair<Stepper, EulerStepper::StatsPtr>
{
    auto stats = std::make_shared<EulerStepper::Stats>();
    return {Stepper{EulerStepper{dt, stats}}, stats};
}

} // namespace mfptlib::test

#endif
