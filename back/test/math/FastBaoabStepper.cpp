// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/math/FastBaoabStepper.hpp>


TEST_CASE("math/FastBaoabStepper", "[math]")
{
    SECTION("FastBaoabStepper throws iff constructed with dt <= 0.")
    {
        REQUIRE_NOTHROW(mfptlib::FastBaoabStepper{1e-3});
        REQUIRE_THROWS_AS(
            (mfptlib::FastBaoabStepper{0.0}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (mfptlib::FastBaoabStepper{-1e-3}),
            std::invalid_argument
        );
    }
}
