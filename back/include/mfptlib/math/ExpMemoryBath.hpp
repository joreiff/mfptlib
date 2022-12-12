// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_EXPMEMORYBATH_HPP
#define MFPTLIB_MATH_EXPMEMORYBATH_HPP

#include <cmath>

#include <pcg_random.hpp>

#include <mfptlib/core/Cache.hpp>
#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

class ExpMemoryBath
{
public:
    explicit ExpMemoryBath(double kb_t, double friction, double memory, Seed seed)
        : noise_{std::sqrt(2 * kb_t * friction)}
        , friction_{friction}
        , memory_{memory}
        , rng_{seed}
    {
        expect(kb_t >= 0.0, "The temperature kb_t must be >= 0.");
        expect(friction >= 0.0, "The friction must be >= 0.");
        expect(memory >= 0.0, "The memory parameter must be >= 0.");
    }

    void apply_forces(VectorsRef momenta, const VectorsCRef& masses, double dt);

    void filter_states(const Booleans& predicate)
    { force_.filter_rows(predicate); }

    void reset() noexcept
    { force_.reset(); }


private:
    double noise_;
    double friction_;
    double memory_;
    pcg64_oneseq rng_;
    Cache<Vectors> force_{};
};

} // namespace mfptlib

#endif
