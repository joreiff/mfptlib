// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/math/BaoabStepper.hpp>


TEST_CASE("math/BaoabStepper", "[math]")
{
    SECTION("BaoabStepper throws iff constructed with dt <= 0.")
    {
        REQUIRE_NOTHROW(mfptlib::BaoabStepper{1e-3});
        REQUIRE_THROWS_AS(
            (mfptlib::BaoabStepper{0.0}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (mfptlib::BaoabStepper{-1e-3}),
            std::invalid_argument
        );
    }
}
