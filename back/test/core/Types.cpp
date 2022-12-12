// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>

#include "../Matcher.hpp"


TEST_CASE("core/Types", "[core]")
{
    SECTION("positions() and momenta() return the correct columns.")
    {
        const mfptlib::Vectors states{
            {1.1, 1.2, 1.3, 1.4},
            {2.1, 2.2, 2.3, 2.4},
            {3.1, 3.2, 3.3, 3.4},
        };

        REQUIRE_THAT(mfptlib::positions(states),
            mfptlib::test::equals({{1.1, 1.2}, {2.1, 2.2}, {3.1, 3.2}}));
        REQUIRE_THAT(mfptlib::momenta(states),
            mfptlib::test::equals({{1.3, 1.4}, {2.3, 2.4}, {3.3, 3.4}}));
    }
}
