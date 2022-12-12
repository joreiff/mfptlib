// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/LangevinBath.hpp>


TEST_CASE("math/LangevinBath", "[math]")
{
    SECTION("LangevinBath throws iff constructed with invalid arguments.")
    {
        REQUIRE_NOTHROW(mfptlib::LangevinBath{1.0, 1.0, 42});
        REQUIRE_NOTHROW(mfptlib::LangevinBath{0.0, 0.0, 42});
        REQUIRE_THROWS_AS(
            (mfptlib::LangevinBath{-1.0, 1.0, 42}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            (mfptlib::LangevinBath{1.0, -1.0, 42}),
            std::invalid_argument
        );
    }
}
