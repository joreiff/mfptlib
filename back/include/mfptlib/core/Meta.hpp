// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_CORE_META_HPP
#define MFPTLIB_CORE_META_HPP

#include <type_traits>


namespace mfptlib {

template<bool Condition>
using EnableIf = std::enable_if_t<Condition, bool>;

template<typename T, typename U>
constexpr bool SameDecay = std::is_same_v<std::decay_t<T>, std::decay_t<U>>;

template<typename Argument, typename Class>
using ProtectSpecial = EnableIf<!SameDecay<Argument, Class>>;

} // namespace mfptlib

#endif
