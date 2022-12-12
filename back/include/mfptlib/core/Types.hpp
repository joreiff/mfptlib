// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_CORE_TYPES_HPP
#define MFPTLIB_CORE_TYPES_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>

#include <Eigen/Dense>


namespace mfptlib {

using Index = Eigen::Index;
using Seed = std::uint64_t;

// Vectors has layout [particle][coordinate], i.e., every row is a particle.
// The storage is column-major, hopefully aiding vectorization.
using Vector = Eigen::Array<double, 1, Eigen::Dynamic>;
using Vectors = Eigen::ArrayXXd;
using VectorsRef = Eigen::Ref<Vectors>;
using VectorsCRef = Eigen::Ref<const Vectors>;
using Scalars = Eigen::ArrayXd;
using ScalarsRef = Eigen::Ref<Scalars>;
using ScalarsCRef = Eigen::Ref<const Scalars>;
using Indices = Eigen::ArrayX<Index>;
using Booleans = Eigen::ArrayX<bool>;


template<typename Array>
auto positions(Array&& states) noexcept -> decltype(auto)
{
    assert(states.cols() % 2 == 0);
    return std::forward<Array>(states).leftCols(states.cols() / 2);
}

template<typename Array>
auto momenta(Array&& states) noexcept -> decltype(auto)
{
    assert(states.cols() % 2 == 0);
    return std::forward<Array>(states).rightCols(states.cols() / 2);
}


template<typename T, typename... Args>
void reconstruct(T& t, Args&&... args)
{
    static_assert(
        std::is_trivially_destructible_v<T>
        or (noexcept(t.~T())
            and noexcept(::new(&t) T(std::forward<Args>(args)...))),
        "Target must be trivially destructible or nothrow de-/constructible."
    );

    t.~T();
    ::new(&t) T(std::forward<Args>(args)...);
}

} // namespace mfptlib

#endif
