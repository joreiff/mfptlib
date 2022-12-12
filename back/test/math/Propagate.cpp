// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <catch2/catch.hpp>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/math/Observer.hpp>
#include <mfptlib/math/Predicate.hpp>
#include <mfptlib/math/Propagate.hpp>
#include <mfptlib/math/Stepper.hpp>
#include <mfptlib/sys/EmptyPlane.hpp>
#include <mfptlib/sys/System.hpp>

#include "../EulerStepper.hpp"
#include "../Matcher.hpp"
#include "../NullBath.hpp"


TEST_CASE("math/Propagate", "[math]")
{
    SECTION("partition_record() permutes according to the predicate array.")
    {
        mfptlib::Indices order{{0, 4, 2, 3, 1, 5}};
        mfptlib::Vectors states{
            {0.1, 0.2},
            {4.1, 4.2},
            {2.1, 2.2},
            {3.1, 3.2},
            {1.1, 1.2},
            {5.1, 5.2},
        };
        const mfptlib::Booleans predicate{{false, true, false, true}};
        const mfptlib::Indices expected_order{{3, 4, 2, 0, 1, 5}};
        const mfptlib::Vectors expected_states{
            {3.1, 3.2},
            {4.1, 4.2},
            {2.1, 2.2},
            {0.1, 0.2},
            {1.1, 1.2},
            {5.1, 5.2},
        };

        const mfptlib::Index partition_point = mfptlib::detail::partition_record(
            order, states.topRows(4), predicate);

        REQUIRE(partition_point == 2);
        REQUIRE_THAT(order, mfptlib::test::equals(expected_order));
        REQUIRE_THAT(states, mfptlib::test::equals(expected_states));
    }

    SECTION("restore_order() permutes according to the order array.")
    {
        mfptlib::Indices order{{3, 4, 2, 0, 1, 5}};
        mfptlib::Vectors states{
            {3.1, 3.2},
            {4.1, 4.2},
            {2.1, 2.2},
            {0.1, 0.2},
            {1.1, 1.2},
            {5.1, 5.2},
        };
        const mfptlib::Indices expected_order{{0, 1, 2, 3, 4, 5}};
        const mfptlib::Vectors expected_states{
            {0.1, 0.2},
            {1.1, 1.2},
            {2.1, 2.2},
            {3.1, 3.2},
            {4.1, 4.2},
            {5.1, 5.2},
        };

        mfptlib::detail::restore_order(order, states);

        REQUIRE_THAT(order, mfptlib::test::equals(expected_order));
        REQUIRE_THAT(states, mfptlib::test::equals(expected_states));
    }

    SECTION("propagate_to() propagates to the desired final time.")
    {
        auto [stepper, stepper_stats] = mfptlib::test::euler_stepper();
        auto [bath, bath_stats] = mfptlib::test::null_bath();
        const mfptlib::System system{mfptlib::EmptyPlane{{{1.0, 2.0}}}};

        mfptlib::Vectors states{
            {0.0, 0.0, 1.0, 1.0},
            {4.0, 2.0, 0.0, 2.0},
            {0.0, 2.0, 3.0, 0.0},
        };
        const mfptlib::Vectors expected_states{
            {3.0, 1.5, 1.0, 1.0},
            {4.0, 5.0, 0.0, 2.0},
            {9.0, 2.0, 3.0, 0.0},
        };

        int num_observed{0};
        const mfptlib::Observer observer{[&](const mfptlib::VectorsCRef&, double)
            { ++num_observed; }};

        const double t_end = mfptlib::propagate_to(
            stepper, bath, system, states, 0.0, 2.5, observer);

        REQUIRE(num_observed == 4);
        REQUIRE(t_end == Approx(3.0).epsilon(mfptlib::test::Epsilon));
        REQUIRE_THAT(states, mfptlib::test::approx(expected_states));
        REQUIRE(stepper_stats->step == 3);
        REQUIRE(stepper_stats->filter_states == 0);
        REQUIRE(stepper_stats->reset == 0);
        REQUIRE(bath_stats->apply_forces == 3);
        REQUIRE(bath_stats->filter_states == 0);
        REQUIRE(bath_stats->reset == 0);
    }

    SECTION("propagate_to() throws if t > t_end.")
    {
        auto&& [stepper, stepper_stats] = mfptlib::test::euler_stepper();
        auto [bath, bath_stats] = mfptlib::test::null_bath();
        const mfptlib::System system{mfptlib::EmptyPlane{{{1.0, 2.0}}}};
        mfptlib::Vectors states{{0.0, 0.0, 1.0, 1.0}};
        const mfptlib::Observer observer{};

        REQUIRE_THROWS_AS(
            mfptlib::propagate_to(
                stepper, bath, system, states, 1.0, 0.0, observer),
            std::invalid_argument
        );
    }

    SECTION("propagate_while() propagates until the predicate returns false.")
    {
        auto&& [stepper, stepper_stats] = mfptlib::test::euler_stepper();
        auto [bath, bath_stats] = mfptlib::test::null_bath();
        const mfptlib::System system{mfptlib::EmptyPlane{{{1.0, 2.0}}}};

        mfptlib::Vectors states{
            {0.0, 0.0, 3.0, 1.0},
            {0.0, 0.0, 1.0, 1.0},
            {1.0, 0.0, 1.5, 2.0},
            {3.0, 0.0, 1.0, 1.0},
        };
        const mfptlib::Vectors expected_states{
            {3.0, 0.5, 3.0, 1.0},
            {3.0, 1.5, 1.0, 1.0},
            {4.0, 2.0, 1.5, 2.0},
            {3.0, 0.0, 1.0, 1.0},
        };
        const mfptlib::Scalars expected_t_end{{1.0, 3.0, 2.0, 0.0}};

        const mfptlib::Predicate predicate{
            [&](const mfptlib::VectorsCRef& s, double) -> mfptlib::Booleans
            { return s.col(0) < 2.9; },
        };

        int num_observed{0};
        const mfptlib::Observer observer{
            [&](const mfptlib::VectorsCRef& s, double)
            {
                REQUIRE(s.rows() == 4 - num_observed);
                ++num_observed;
            },
        };

        const mfptlib::Scalars t_end = mfptlib::propagate_while(
            stepper, bath, system, states, 0.0, predicate, observer);

        REQUIRE(num_observed == 4);
        REQUIRE_THAT(t_end, mfptlib::test::approx(expected_t_end));
        REQUIRE_THAT(states, mfptlib::test::approx(expected_states));
        REQUIRE(stepper_stats->step == 3);
        REQUIRE(stepper_stats->filter_states == 3);
        REQUIRE(stepper_stats->reset == 0);
        REQUIRE(bath_stats->apply_forces == 3);
        REQUIRE(bath_stats->filter_states == 3);
        REQUIRE(bath_stats->reset == 0);
    }
}
