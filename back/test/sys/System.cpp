// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <cmath>
#include <stdexcept>
#include <type_traits>

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/sys/HarmonicOscillator.hpp>
#include <mfptlib/sys/System.hpp>

#include "../Matcher.hpp"


TEST_CASE("sys/System", "[sys]")
{
    const double sqrt2 = std::sqrt(2.0);

    constexpr double t{0.0};
    const mfptlib::Vectors states{
        { 0.0, 0.0  ,  1.0,  2.0  },
        { 1.0, sqrt2,  0.0,  sqrt2},
        { 0.0, 2.0  , -1.0,  0.0  },
        {-1.0, sqrt2,  0.0, -sqrt2},
    };
    const mfptlib::Scalars expected_potential{{
        0.0 + 0.0,
        2.0 + 1.0,
        0.0 + 2.0,
        2.0 + 1.0,
    }};
    const mfptlib::Scalars expected_kinetic_energy{{
        0.5 + 1.0,
        0.0 + 0.5,
        0.5 + 0.0,
        0.0 + 0.5,
    }};
    const mfptlib::Vectors expected_force{
        { 0.0,  0.0  },
        {-4.0, -sqrt2},
        { 0.0, -2.0  },
        { 4.0, -sqrt2},
    };
    const mfptlib::Vectors expected_masses{
        {1.0, 2.0},
        {1.0, 2.0},
        {1.0, 2.0},
        {1.0, 2.0},
    };

    const auto verify = [&](const mfptlib::System& system)
    {
        REQUIRE_THAT(
            potential(system, states, t),
            mfptlib::test::approx(expected_potential)
        );
        REQUIRE_THAT(
            kinetic_energy(system, states),
            mfptlib::test::approx(expected_kinetic_energy)
        );
        REQUIRE_THAT(
            total_energy(system, states, t),
            mfptlib::test::approx(expected_potential + expected_kinetic_energy)
        );
        REQUIRE_THAT(
            force(system, states, t),
            mfptlib::test::approx(expected_force)
        );
        REQUIRE_THAT(
            masses(system, states),
            mfptlib::test::approx(expected_masses)
        );
    };

    SECTION("System forwards calls to the underlying implementation.")
    {
        const mfptlib::System system{mfptlib::HarmonicOscillator{
            {{1.0, 2.0}}, {{4.0, 1.0}}}};
        verify(system);
    }

    static_assert(!std::is_copy_constructible_v<mfptlib::System>,
        "System should not be copy-constructible.");
    static_assert(!std::is_copy_assignable_v<mfptlib::System>,
        "System should not be copy-assignable.");

    SECTION("System can be moved.")
    {
        mfptlib::System system{mfptlib::HarmonicOscillator{
            {{1.0, 2.0}}, {{4.0, 1.0}}}};
        const mfptlib::System moved{std::move(system)};
        verify(moved);
    }

    SECTION("System throws if incompatible state/system size is used.")
    {
        const mfptlib::System system{mfptlib::HarmonicOscillator{
            {{1.0, 2.0, 3.0}}, {{4.0, 3.0, 2.0}}}};

        REQUIRE_THROWS_AS(
            potential(system, states, t),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            kinetic_energy(system, states),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            total_energy(system, states, t),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            force(system, states, t),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            masses(system, states),
            std::invalid_argument
        );
    }
}
