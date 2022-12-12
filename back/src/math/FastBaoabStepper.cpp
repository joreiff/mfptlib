// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <mfptlib/math/FastBaoabStepper.hpp>


namespace mfptlib {

/**
 * Langevin BAOAB integrator:
 *
 * ζ = √[k_B T (1 - e^{-2γh})]
 * R_n is standard normally-distributed random variable (μ = 0, σ = 1)
 * p_{n + 1/4} = p_n + h/2 * F(q_n)                                         (B1)
 * q_{n + 1/2} = q_n + h/2 M^{-1} p_{n + 1/4}                               (A1)
 * p_{n + 3/4} = e^{-hγ} p_{n + 1/4} + ζ M^{1/2} R_n                        (O)
 * q_{n + 1} = q_{n + 1/2} + h/2 M^{-1} p_{n + 3/4}                         (A2)
 * p_{n + 1} = p_{n + 3/4} + h/2 F(q_{n + 1})                               (B2)
 *
 * This version caches the forces evaluated in (B2) for use in (B1).
 *
 * References:
 * - Leimkuhler et al., Molecular Dynamics (2015):
 *   https://doi.org/10.1007/978-3-319-16375-8
 * - Fass et al., Entropy 20(5), 318 (2018):
 *   https://doi.org/10.3390/e20050318
 */
void FastBaoabStepper::step(
    Bath& bath, const System& system, VectorsRef states, double& t
)
{
    const double half_dt = 0.5 * dt_;
    VectorsRef q = positions(states);
    VectorsRef p = momenta(states);

    if(force_.rows() == 0)
        p += half_dt * force(system, states, t);                         // (B1)
    else
    {
        expect(
            force_.rows() == p.rows() and force_.cols() == p.cols(),
            "Size of the passed states is incompatible with the cached forces. "
            "Did you forget to call Stepper.reset() or Stepper.filter_states()?"
        );
        p += *force_;                                                    // (B1)
    }

    q += half_dt * p / masses(system, states);                           // (A1)
    bath.apply_forces(p, masses(system, states), dt_);                   // (O)
    q += half_dt * p / masses(system, states);                           // (A2)
    force_ = half_dt * force(system, states, t);
    p += *force_;                                                        // (B2)

    t += dt_;
}

} // namespace mfptlib
