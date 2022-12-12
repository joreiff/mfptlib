// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <catch2/catch.hpp>

#include <mfptlib/core/Cache.hpp>
#include <mfptlib/core/Types.hpp>

#include "../Matcher.hpp"


TEST_CASE("core/Cache", "[core]")
{
    SECTION("An empty Cache can be constructed.")
    {
        mfptlib::Cache<mfptlib::Vectors> cache{};
        REQUIRE(cache.rows() == 0);
        REQUIRE(cache.cols() == 0);
        REQUIRE(cache.size() == 0);
        REQUIRE_THAT(*cache, mfptlib::test::equals<double>({}));

        SECTION("Data can be assigned.")
        {
            cache = mfptlib::Vectors{
                {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {7.0, 8.0}};
            REQUIRE(cache.rows() == 4);
            REQUIRE(cache.cols() == 2);
            REQUIRE(cache.size() == 8);
            REQUIRE_THAT(*cache, mfptlib::test::equals(
                {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}, {7.0, 8.0}}));

            SECTION("Data can be modified.")
            {
                *cache += 0.1;
                REQUIRE(cache.rows() == 4);
                REQUIRE(cache.cols() == 2);
                REQUIRE(cache.size() == 8);
                REQUIRE_THAT(*cache, mfptlib::test::equals(
                    {{1.1, 2.1}, {3.1, 4.1}, {5.1, 6.1}, {7.1, 8.1}}));
            }

            SECTION("Data can be overwritten with a different shape.")
            {
                cache = mfptlib::Vectors{
                    {1.1, 2.1, 3.1}, {4.1, 5.1, 6.1}, {7.1, 8.1, 9.1}};
                REQUIRE(cache.rows() == 3);
                REQUIRE(cache.cols() == 3);
                REQUIRE(cache.size() == 9);
                REQUIRE_THAT(*cache, mfptlib::test::equals(
                    {{1.1, 2.1, 3.1}, {4.1, 5.1, 6.1}, {7.1, 8.1, 9.1}}));
            }

            SECTION("Rows can be filtered.")
            {
                cache.filter_rows({{true, false, true, true}});
                REQUIRE(cache.rows() == 3);
                REQUIRE(cache.cols() == 2);
                REQUIRE(cache.size() == 6);
                REQUIRE_THAT(*cache, mfptlib::test::equals(
                    {{1.0, 2.0}, {7.0, 8.0}, {5.0, 6.0}}));
            }

            SECTION("A cache can be reset.")
            {
                cache.reset();
                REQUIRE(cache.rows() == 0);
                REQUIRE(cache.cols() == 0);
                REQUIRE(cache.size() == 0);
                REQUIRE_THAT(*cache, mfptlib::test::equals<double>({}));
            }
        }
    }
}
