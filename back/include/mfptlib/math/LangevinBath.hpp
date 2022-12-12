// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_LANGEVINBATH_HPP
#define MFPTLIB_MATH_LANGEVINBATH_HPP

#include <cmath>

#include <pcg_random.hpp>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

class LangevinBath
{
public:
    explicit LangevinBath(double kb_t, double friction, Seed seed)
        : sqrt_kb_t_{std::sqrt(kb_t)}
        , friction_{friction}
        , rng_{seed}
    {
        expect(kb_t >= 0.0, "The temperature kb_t must be >= 0.");
        expect(friction >= 0.0, "The friction must be >= 0.");
    }

    void apply_forces(
        VectorsRef momenta, const VectorsCRef& masses, double dt
    ) noexcept;


private:
    double sqrt_kb_t_;
    double friction_;
    pcg64_oneseq rng_;
};

} // namespace mfptlib

#endif
