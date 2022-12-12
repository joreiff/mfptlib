#!/usr/bin/env python3

import contextlib
import sys

import mfptlib
import numpy as np
import scipy.constants
import scipy.optimize
import tqdm


# The LiNC ⇌ LiCN potential uses Hartree atomic units.
KB = scipy.constants.value('kelvin-hartree relationship')

# We use a very high temperature so that the calculation is quick.
BATH_TEMPERATURE = 5500
BATH_FRICTION = 2e-4
BATH_SEED = 42
ENSEMBLE_SIZE = 2048
ENSEMBLE_SEED = 43
STEPPER_DT = 0.1


def main():
    kb_t = KB * BATH_TEMPERATURE

    # Configure the simulation:
    stepper = mfptlib.lf_middle_stepper(STEPPER_DT)
    bath = mfptlib.langevin_bath(kb_t, BATH_FRICTION, BATH_SEED)
    system = mfptlib.lithium_cyanide()
    ensemble = delta_ensemble(system, ENSEMBLE_SIZE, kb_t, ENSEMBLE_SEED)
    t_0 = 0.0
    predicate = mfptlib.Predicate(is_reacting)

    # Run the simulation (including a nice progress bar):
    with track_progress(ENSEMBLE_SIZE) as observer:
        t_end = mfptlib.propagate_while(
            stepper, bath, system, ensemble, t_0, predicate, observer)

    # Calculate the MFPT and report the results:
    mfpt = np.mean(t_end - t_0)
    print(f'MFPT: {mfpt} a.u.')
    print(f'Rate: {1 / mfpt} a.u.')


def is_reacting(qp, t):
    return np.abs(mfptlib.positions[qp][..., 0]) < 0.6 * np.pi


def licn_minimum(system):
    theta = 0.0
    pot = lambda r: system.potential(qp=np.array([(theta, r[0], 0.0, 0.0)]), t=0.0)[0]
    result = scipy.optimize.minimize(pot, x0=4.5, tol=1e-8)
    assert result.success
    return (theta, result.x[0])


def delta_ensemble(system, size, kb_t, seed):
    q0 = np.repeat([licn_minimum(system)], size, axis=0)
    return mfptlib.maxwell_boltzmann_ensemble(system, kb_t, q0, seed)


@contextlib.contextmanager
def track_progress(ensemble_size):
    with tqdm.tqdm(total=ensemble_size, miniters=1) as progress:
        yield mfptlib.Observer(lambda qp, t:
            progress.update(ensemble_size - len(qp) - progress.n))
        progress.update(ensemble_size - progress.n)


if __name__ == '__main__':
    sys.exit(int(main() or 0))
