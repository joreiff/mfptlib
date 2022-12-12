// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <mfptlib/math/LangevinBath.hpp>

#include <random>


namespace mfptlib {

void LangevinBath::apply_forces(
    VectorsRef momenta, const VectorsCRef& masses, double dt
) noexcept
{
    const double friction_scale = std::exp(-friction_ * dt);
    const double noise_scale = std::sqrt(1 - friction_scale * friction_scale);
    std::normal_distribution normal{0.0, sqrt_kb_t_ * noise_scale};

    momenta *= friction_scale;
    momenta += masses.sqrt() * Vectors::NullaryExpr(
        masses.rows(), masses.cols(), [&](){ return normal(rng_); });
}

} // namespace mfptlib
