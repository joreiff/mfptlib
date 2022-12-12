# Copyright 2022 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import numpy as np
import pytest

import mfptlib

from .sys import lithium_cyanide as licn


POT_TOLERANCE = 1e-12
GRAD_TOLERANCE = 1e-8
GRAD_EPSILON = 1e-6
POINTS_PER_DOF = 1000
SYSTEMS = {
    'EmptyPlane': (
        mfptlib.empty_plane(masses=[1.0, 2.0]),
        [(-1.0, +1.0), (-1.0, +1.0)],
    ),
    'HarmonicOscillator': (
        mfptlib.harmonic_oscillator(masses=[1.0, 2.0], strengths=[1.0, 1.5]),
        [(-1.0, +1.0), (-1.0, +1.0)],
    ),
    'LithiumCyanide': (
        mfptlib.lithium_cyanide(),
        [(0.0, np.pi), (3.0, 5.5)],
    ),
}


@pytest.mark.parametrize(['sys', 'extent'], SYSTEMS.values(), ids=SYSTEMS.keys())
def test_force(sys, extent):
    states = states_on_grid(extent)
    analytical = sys.force(states, t=0.0)
    numerical = grad_force(sys, states, t=0.0)

    # pytest.approx is very slow for large arrays
    assert np.abs(analytical - numerical).max() <= GRAD_TOLERANCE


def test_lithium_cyanide():
    sys, extent = SYSTEMS['LithiumCyanide']
    states = states_on_grid(extent)
    cpp = sys.potential(states, t=0.0)
    py = licn.potential(states)

    assert np.abs(cpp - py).max() <= POT_TOLERANCE


def states_on_grid(extent):
    ranges = (np.linspace(*lim, POINTS_PER_DOF) for lim in extent)
    return mfptlib.states(q=mfptlib.grid(*ranges))


def grad_force(sys, states, t, eps=GRAD_EPSILON):
    n = mfptlib.dofs(states)
    pot = lambda i, delta: sys.potential(states + np.eye(1, 2 * n, i) * delta, t)

    return np.stack(
        [(pot(i, -eps) - pot(i, +eps)) / (2 * eps) for i in range(n)],
        axis=1,
    )
