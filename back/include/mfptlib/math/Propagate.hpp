// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_PROPAGATE_HPP
#define MFPTLIB_MATH_PROPAGATE_HPP

#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Observer.hpp>
#include <mfptlib/math/Predicate.hpp>
#include <mfptlib/math/Stepper.hpp>
#include <mfptlib/sys/System.hpp>


namespace mfptlib {

namespace detail {

auto partition_record(
    Indices& order, VectorsRef states, const Booleans& predicate
) noexcept -> Index;

void restore_order(Indices& order, VectorsRef states) noexcept;

} // namespace detail


auto propagate_to(
    Stepper& stepper, Bath& bath, const System& system,
    VectorsRef states, double t, double t_end,
    const Observer& observe
) -> double;

auto propagate_while(
    Stepper& stepper, Bath& bath, const System& system,
    VectorsRef states, double t, const Predicate& predicate,
    const Observer& observe
) -> Scalars;

} // namespace mfptlib

#endif
