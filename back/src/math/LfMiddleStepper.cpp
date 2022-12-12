// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <mfptlib/math/LfMiddleStepper.hpp>


namespace mfptlib {

/**
 * Langevin LF-Middle integrator:
 *
 * ζ = √[k_B T (1 - e^{-2γh})]
 * R_n is standard normally-distributed random variable (μ = 0, σ = 1)
 * p'_{n + 1/2} = p_{n - 1/2} + h * F(q_n)                                  (p)
 * q_{n + 1/2} = q_n + h/2 M^{-1} p'_{n + 1/2}                              (x1)
 * p_{n + 1/2} = e^{-hγ} p'_{n + 1/2} + ζ M^{1/2} R_n                       (T)
 * q_{n + 1} = q_{n + 1/2} + h/2 M^{-1} p_{n + 1/2}                         (x2)
 *
 * References:
 * - Zhang et al., J. Phys. Chem. A 123, 6056-6079 (2019):
 *   https://doi.org/10.1021/acs.jpca.9b02771
 */
void LfMiddleStepper::step(
    Bath& bath, const System& system, VectorsRef states, double& t
)
{
    const double half_dt = 0.5 * dt_;
    VectorsRef q = positions(states);
    VectorsRef p = momenta(states);

    p += dt_ * force(system, states, t);                                 // (p)
    q += half_dt * p / masses(system, states);                           // (x1)
    bath.apply_forces(p, masses(system, states), dt_);                   // (T)
    q += half_dt * p / masses(system, states);                           // (x2)

    t += dt_;
}

} // namespace mfptlib
