// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_LFMIDDLESTEPPER_HPP
#define MFPTLIB_MATH_LFMIDDLESTEPPER_HPP

#include <utility>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/sys/System.hpp>


namespace mfptlib {

class LfMiddleStepper
{
public:
    explicit LfMiddleStepper(double dt)
        : dt_{dt}
    { expect(dt > 0.0, "Step size dt must be > 0."); }

    void step(Bath& bath, const System& system, VectorsRef states, double& t);


private:
    double dt_;
};

} // namespace mfptlib

#endif
