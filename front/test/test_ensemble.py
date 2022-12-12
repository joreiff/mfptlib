# Copyright 2022 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import numpy as np
import pytest
import scipy.optimize

import mfptlib


KB_T = 10.0
ENSEMBLE_SIZE = 65_536
SYSTEM = mfptlib.empty_plane(masses=[2.0, 2.0])
SEED = 42


def test_random_gen():
    rng = np.random.default_rng()
    assert mfptlib.random_gen(rng) is rng
    assert isinstance(mfptlib.random_gen(SEED), np.random.Generator)


def test_maxwell_boltzmann_ensemble():
    q = np.zeros((ENSEMBLE_SIZE, 2))
    qp = mfptlib.maxwell_boltzmann_ensemble(SYSTEM, KB_T, q, SEED)
    kin_enrg = SYSTEM.kinetic_energy(qp)
    hist, bins = np.histogram(kin_enrg, bins=60, range=(0, 60), density=True)
    fit_estimate = scipy.optimize.curve_fit(
        thermal_energy_2d, 0.5 * (bins[:-1] + bins[1:]), hist)[0][0]
    avrg_estimate = np.mean(kin_enrg)

    assert fit_estimate == pytest.approx(KB_T, rel=0.01)
    assert avrg_estimate == pytest.approx(KB_T, rel=0.01)


def thermal_energy_2d(energy, kb_t):
    return np.exp(-energy / kb_t) / kb_t
