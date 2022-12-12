// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Observer.hpp>

#include "../Matcher.hpp"


TEST_CASE("math/Observer", "[math]")
{
    SECTION("Observer forwards calls to the underlying implementation.")
    {
        const double time{1.0};
        const mfptlib::Vectors states{
            {1.1, 1.2, 1.0, 1.4},
            {2.1, 2.2, 2.0, 2.4},
            {3.1, 3.2, 3.0, 3.4},
        };

        double observed_time{0.0};
        mfptlib::Vectors observed_states{};
        const mfptlib::Observer pred{[&](const mfptlib::VectorsCRef& s, double t)
            { observed_states = s; observed_time = t; }};
        pred(states, time);

        REQUIRE_THAT(observed_states, mfptlib::test::equals(states));
        REQUIRE(observed_time == time);
    }

    SECTION("Observer can be constructed with an empty function.")
    {
        REQUIRE_NOTHROW(mfptlib::Observer{{}});
    }
}
