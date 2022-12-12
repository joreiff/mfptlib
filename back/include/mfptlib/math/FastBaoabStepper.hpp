// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_FASTBAOABSTEPPER_HPP
#define MFPTLIB_MATH_FASTBAOABSTEPPER_HPP

#include <optional>
#include <utility>

#include <mfptlib/core/Cache.hpp>
#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/sys/System.hpp>


namespace mfptlib {

class FastBaoabStepper
{
public:
    explicit FastBaoabStepper(double dt)
        : dt_{dt}
    { expect(dt > 0.0, "Step size dt must be > 0."); }

    void step(Bath& bath, const System& system, VectorsRef states, double& t);

    void filter_states(const Booleans& predicate)
    { force_.filter_rows(predicate); }

    void reset() noexcept
    { force_.reset(); }


private:
    double dt_;
    Cache<Vectors> force_;
};

} // namespace mfptlib

#endif
