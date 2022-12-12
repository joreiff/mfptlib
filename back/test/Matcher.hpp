// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_TEST_MATCHER_HPP
#define MFPTLIB_TEST_MATCHER_HPP

#include <initializer_list>
#include <limits>
#include <sstream>
#include <type_traits>
#include <utility>

#include <catch2/catch.hpp>
#include <Eigen/Dense>


namespace mfptlib::test {

namespace detail {

template<typename Dense>
using ScalarFor = typename std::decay_t<Dense>::Scalar;

template<typename Dense>
using ArrayCRef = Eigen::Ref<const Eigen::ArrayXX<ScalarFor<Dense>>>;


template<typename Dense>
class EigenMatcher : public Catch::MatcherBase<ArrayCRef<Dense>>
{
public:
    explicit EigenMatcher(Dense rhs, ScalarFor<Dense> prec) noexcept
        : rhs_{std::forward<Dense>(rhs)}
        , prec_{prec}
    {}

    auto match(const ArrayCRef<Dense>& lhs) const noexcept -> bool override
    { return lhs.isApprox(rhs_, prec_); }

    auto describe() const noexcept -> std::string override
    {
        std::ostringstream ss;
        ss << "\nequals with precision " << prec_ << ":\n" << rhs_;
        return ss.str();
    }

private:
    Dense rhs_;
    ScalarFor<Dense> prec_;
};

template<typename Dense>
EigenMatcher(const Dense&, ScalarFor<Dense>) -> EigenMatcher<const Dense&>;

template<typename Dense>
EigenMatcher(Dense&&, ScalarFor<Dense>) -> EigenMatcher<Dense>;

} // namespace detail


constexpr double Epsilon = std::numeric_limits<double>::epsilon();


template<typename Dense>
auto approx(
    Dense&& rhs,
    detail::ScalarFor<Dense> prec =
        std::numeric_limits<detail::ScalarFor<Dense>>::epsilon()
) noexcept
{
    return detail::EigenMatcher{std::forward<Dense>(rhs), prec};
}

template<typename Scalar>
auto approx(
    std::initializer_list<Scalar> rhs,
    Scalar prec = std::numeric_limits<Scalar>::epsilon()
) noexcept
{
    return detail::EigenMatcher{Eigen::ArrayX<Scalar>{rhs}, prec};
}

template<typename Scalar>
auto approx(
    std::initializer_list<std::initializer_list<Scalar>> rhs,
    Scalar prec = std::numeric_limits<Scalar>::epsilon()
) noexcept
{
    return detail::EigenMatcher{Eigen::ArrayXX<Scalar>{rhs}, prec};
}


template<typename Dense>
auto equals(Dense&& rhs) noexcept
{
    return detail::EigenMatcher{
        std::forward<Dense>(rhs), detail::ScalarFor<Dense>{0}};
}

template<typename Scalar>
auto equals(std::initializer_list<Scalar> rhs) noexcept
{
    return detail::EigenMatcher{Eigen::ArrayX<Scalar>{rhs}, Scalar{0}};
}

template<typename Scalar>
auto equals(std::initializer_list<std::initializer_list<Scalar>> rhs) noexcept
{
    return detail::EigenMatcher{Eigen::ArrayXX<Scalar>{rhs}, Scalar{0}};
}

} // namespace mfptlib::test

#endif
