// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/math/LfMiddleStepper.hpp>


TEST_CASE("math/LfMiddleStepper", "[math]")
{
    SECTION("LfMiddleStepper throws iff constructed with dt <= 0.")
    {
        REQUIRE_NOTHROW(mfptlib::LfMiddleStepper{1e-3});
        REQUIRE_THROWS_AS(
            (mfptlib::LfMiddleStepper{0.0}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (mfptlib::LfMiddleStepper{-1e-3}),
            std::invalid_argument
        );
    }
}
