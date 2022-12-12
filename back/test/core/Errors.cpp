// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <stdexcept>

#include <catch2/catch.hpp>

#include <mfptlib/core/Errors.hpp>


TEST_CASE("core/Errors", "[core]")
{
    SECTION("expect() throws iff the condition is false.")
    {
        REQUIRE_NOTHROW(mfptlib::expect(true, "should be fine"));
        REQUIRE_THROWS_WITH(mfptlib::expect(false, "some error"), "some error");
        REQUIRE_THROWS_MATCHES(
            mfptlib::expect<std::runtime_error>(false, "runtime error"),
            std::runtime_error, Catch::Matchers::Message("runtime error")
        );
    }
}
