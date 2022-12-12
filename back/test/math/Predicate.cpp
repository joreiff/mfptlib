// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Predicate.hpp>

#include "../Matcher.hpp"


TEST_CASE("math/Predicate", "[math]")
{
    const double t{1.0};
    const mfptlib::Vectors states{
        {1.1, 1.2, 1.0, 1.4},
        {2.1, 2.2, 2.0, 2.4},
        {3.1, 3.2, 3.0, 3.4},
    };

    SECTION("Predicate forwards calls to the underlying implementation.")
    {
        const mfptlib::Predicate pred{[](const mfptlib::VectorsCRef& s, double)
            { return s.col(0) >= 2.0; }};

        REQUIRE_THAT(
            pred(states, t),
            mfptlib::test::equals({false, true, true})
        );
    }

    SECTION("Predicate throws if constructed with an empty function.")
    {
        REQUIRE_THROWS_AS(
            mfptlib::Predicate{{}},
            std::invalid_argument
        );
    }

    SECTION("Predicate throws if the callback returns an array of invalid size.")
    {
        const mfptlib::Predicate pred{[](const mfptlib::VectorsCRef& s, double)
            { return s.block<2, 1>(0, 0) >= 2.0; }};

        REQUIRE_THROWS_AS(
            pred(states, t),
            std::invalid_argument
        );
    }
}
