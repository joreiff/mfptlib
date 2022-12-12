// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <mfptlib/math/ExpMemoryBath.hpp>

#include <random>


namespace mfptlib {

/**
 * GLE bath with a simple Prony-type (🦄) memory kernel.
 * Based on the multistage splitting by Baczewski and Bond with ξ = 1.
 *
 * References:
 * - Baczewski and Bond, J. Chem. Phys. 139, 044107 (2013):
 *   https://doi.org/10.1063/1.4815917
 * - Leimkuhler et al., Molecular Dynamics (2015):
 *   https://doi.org/10.1007/978-3-319-16375-8
 * - Leimkuhler and Sachs, SIAM J. Sci. Comput. 44:1, A364-A388, (2022):
 *   doi.org/10.1137/20M138497X
 * - Duong and Shang, J. Comp. Phys. 464, 111332 (2022):
 *   https://doi.org/10.1016/j.jcp.2022.111332
 */
void ExpMemoryBath::apply_forces(
    VectorsRef momenta, const VectorsCRef& masses, double dt
)
{
    if(force_.rows() == 0)
        force_ = Vectors::Zero(momenta.rows(), momenta.cols());
    else
        expect(
            force_.rows() == momenta.rows() and force_.cols() == momenta.cols(),
            "Size of the passed momenta is incompatible with the cached forces. "
            "Did you forget to call Bath.reset() or Bath.filter_states()?"
        );

    const double tau = friction_ * memory_;
    const double h = 0.5 * dt;
    const double memory_scale = std::exp(-dt / tau);
    const double noise_scale = std::sqrt(
        (1 - memory_scale) * (1 - memory_scale) / dt);

    std::normal_distribution normal{0.0, noise_ * noise_scale};

    momenta += h * *force_;
    *force_ *= memory_scale;
    *force_ -= (1 - memory_scale) * friction_ * momenta;
    *force_ += masses.sqrt() * Vectors::NullaryExpr(
        masses.rows(), masses.cols(), [&](){ return normal(rng_); });
    momenta += h * *force_;
}

} // namespace mfptlib
