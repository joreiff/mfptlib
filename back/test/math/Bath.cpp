// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <type_traits>
#include <utility>

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>

#include "../Matcher.hpp"


namespace mfptlib::test { namespace {

struct DummyBath
{
    double friction;

    void apply_forces(VectorsRef momenta, const VectorsCRef& masses, double dt)
    {
        momenta *= 1 - friction;
        momenta += masses * dt;
    }
};

} } // namespace mfptlib::test


TEST_CASE("math/Bath", "[math]")
{
    const double dt = 0.1;
    const mfptlib::Vectors masses{
        {1.0, 2.0},
        {2.0, 3.0},
        {3.0, 4.0},
    };
    mfptlib::Vectors states{
        {-1.0, -1.0, 1.0, 1.2},
        {-1.0, -1.0, 2.0, 2.2},
        {-1.0, -1.0, 3.0, 3.2},
    };
    const mfptlib::Vectors expected_states{
        {-1.0, -1.0, 0.6, 0.8},
        {-1.0, -1.0, 1.2, 1.4},
        {-1.0, -1.0, 1.8, 2.0},
    };

    mfptlib::Bath bath{mfptlib::test::DummyBath{0.5}};

    SECTION("Bath forwards calls to the underlying implementation.")
    {
        bath.apply_forces(mfptlib::momenta(states), masses, dt);
        REQUIRE_THAT(states, mfptlib::test::approx(expected_states));
    }

    static_assert(!std::is_copy_constructible_v<mfptlib::Bath>,
        "Bath should not be copy-constructible.");
    static_assert(!std::is_copy_assignable_v<mfptlib::Bath>,
        "Bath should not be copy-assignable.");

    SECTION("Bath can be moved.")
    {
        mfptlib::Bath moved{std::move(bath)};
        moved.apply_forces(mfptlib::momenta(states), masses, dt);
        REQUIRE_THAT(states, mfptlib::test::approx(expected_states));
    }
}
