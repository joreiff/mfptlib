// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>
#include <type_traits>

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/math/Stepper.hpp>
#include <mfptlib/sys/EmptyPlane.hpp>
#include <mfptlib/sys/System.hpp>

#include "../EulerStepper.hpp"
#include "../Matcher.hpp"
#include "../NullBath.hpp"


TEST_CASE("math/Stepper", "[math]")
{
    double t{1.0};
    mfptlib::Vectors states{
        {1.1, 1.2, 1.0, 1.4},
        {2.1, 2.2, 2.0, 2.4},
        {3.1, 3.2, 3.0, 3.4},
    };
    const mfptlib::Vectors expected_states{
        {2.1, 1.9, 1.0, 1.4},
        {4.1, 3.4, 2.0, 2.4},
        {6.1, 4.9, 3.0, 3.4},
    };

    auto [stepper, stepper_stats] = mfptlib::test::euler_stepper();
    auto [bath, bath_stats] = mfptlib::test::null_bath();

    SECTION("Stepper forwards calls to the underlying implementation.")
    {
        const mfptlib::System system{mfptlib::EmptyPlane{{{1.0, 2.0}}}};
        stepper.step(bath, system, states, t);
        stepper.filter_states({{true, false, true}});
        stepper.reset();
        REQUIRE_THAT(states, mfptlib::test::approx(expected_states));
        REQUIRE(t == 2.0);
        REQUIRE(stepper_stats->step == 1);
        REQUIRE(stepper_stats->filter_states == 1);
        REQUIRE(stepper_stats->reset == 1);
        REQUIRE(bath_stats->apply_forces == 1);
        REQUIRE(bath_stats->filter_states == 0);
        REQUIRE(bath_stats->reset == 0);
    }

    static_assert(!std::is_copy_constructible_v<mfptlib::Stepper>,
        "Stepper should not be copy-constructible.");
    static_assert(!std::is_copy_assignable_v<mfptlib::Stepper>,
        "Stepper should not be copy-assignable.");

    SECTION("Stepper can be moved.")
    {
        mfptlib::System system{mfptlib::EmptyPlane{{{1.0, 2.0}}}};
        const mfptlib::System moved{std::move(system)};
        stepper.step(bath, moved, states, t);
        REQUIRE_THAT(states, mfptlib::test::approx(expected_states));
        REQUIRE(t == 2.0);
    }

    SECTION("Stepper throws if constructed with incompatible state/system size.")
    {
        constexpr auto system = [](auto... args)
            { return mfptlib::System{mfptlib::EmptyPlane{{{args...}}}}; };

        REQUIRE_THROWS_AS(
            stepper.step(bath, system(1.0), states, t),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            stepper.step(bath, system(1.0, 2.0, 3.0), states, t),
            std::invalid_argument
        );
    }
}
