// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_TEST_NULLBATH_HPP
#define MFPTLIB_TEST_NULLBATH_HPP

#include <cstddef>

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>


namespace mfptlib::test {

struct NullBath
{
    struct Stats
    {
        std::size_t apply_forces{};
        std::size_t filter_states{};
        std::size_t reset{};
    };
    using StatsPtr = std::shared_ptr<Stats>;

    StatsPtr stats;

    void apply_forces(VectorsRef, const VectorsCRef&, double) noexcept
    { ++stats->apply_forces; }

    void filter_states(const Booleans&) noexcept
    { ++stats->filter_states; }

    void reset() noexcept
    { ++stats->reset; }
};


inline auto null_bath() noexcept
    -> std::pair<Bath, NullBath::StatsPtr>
{
    auto stats = std::make_shared<NullBath::Stats>();
    return {Bath{NullBath{stats}}, stats};
}

} // namespace mfptlib::test

#endif
