// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_SYS_LITHIUMCYANIDE_HPP
#define MFPTLIB_SYS_LITHIUMCYANIDE_HPP

#include <mfptlib/core/Types.hpp>


namespace mfptlib {

struct LithiumCyanide {};


[[nodiscard]]
constexpr auto degrees_of_freedom(const LithiumCyanide&) noexcept -> Index
{ return 2; }


[[nodiscard]]
auto potential(
    const LithiumCyanide& model, const VectorsCRef& states, double t
) noexcept -> Scalars;

[[nodiscard]]
auto force(
    const LithiumCyanide& model, const VectorsCRef& states, double t
) noexcept -> Vectors;

[[nodiscard]]
auto masses(
    const LithiumCyanide& model, const VectorsCRef& states
) noexcept -> Vectors;

} // namespace mfptlib


#endif
