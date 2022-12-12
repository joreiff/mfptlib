# Copyright 2022 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import numpy as np
import pytest
import scipy.optimize

import mfptlib


KB_T = 10
FRICTION = 1e-1
MEMORY = 10 # τ = 1
BATH_SEED = 42
TIME_STEP = 1e-3
TIME_THERMALIZE = 1e2
ENSEMBLE_SIZE = 1024
NUM_ENSEMBLES = 8
SYSTEM = mfptlib.harmonic_oscillator(masses=[2.0, 2.0], strengths=[1.0, 1.0])
STEPPERS = {
    'BaoabStepper': (mfptlib.baoab_stepper(TIME_STEP),),
    'FastBaoabStepper': (mfptlib.fast_baoab_stepper(TIME_STEP),),
    'LfMiddleStepper': (mfptlib.lf_middle_stepper(TIME_STEP),),
}


@pytest.mark.parametrize(['stepper'], STEPPERS.values(), ids=STEPPERS.keys())
def test_temperature_le(stepper):
    bath = mfptlib.langevin_bath(KB_T, FRICTION, BATH_SEED)
    fit_estimate, avrg_estimate = estimate_kb_t(stepper, bath)
    assert fit_estimate == pytest.approx(KB_T, rel=0.01)
    assert avrg_estimate == pytest.approx(KB_T, rel=0.01)


@pytest.mark.parametrize(['stepper'], STEPPERS.values(), ids=STEPPERS.keys())
def test_temperature_gle(stepper):
    bath = mfptlib.exp_memory_bath(KB_T, FRICTION, MEMORY, BATH_SEED)
    fit_estimate, avrg_estimate = estimate_kb_t(stepper, bath)
    assert fit_estimate == pytest.approx(KB_T, rel=0.01)
    assert avrg_estimate == pytest.approx(KB_T, rel=0.01)


def estimate_kb_t(stepper, bath):
    estimates = np.array([
        sample_kb_t(stepper, bath) for _ in range(NUM_ENSEMBLES)
    ])
    return np.mean(estimates[:, 0]), np.mean(estimates[:, 1])


def sample_kb_t(stepper, bath):
    qp = np.array(ENSEMBLE_SIZE * [[0.0, 0.0, 0.0, 0.0]], order='F')
    mfptlib.propagate_to(stepper, bath, SYSTEM, qp, 0.0, TIME_THERMALIZE)
    kin_enrg = SYSTEM.kinetic_energy(qp)
    hist, bins = np.histogram(kin_enrg, bins=60, range=(0, 60), density=True)
    popt, _ = scipy.optimize.curve_fit(
        thermal_energy_2d, 0.5 * (bins[:-1] + bins[1:]), hist)
    return popt[0], np.mean(kin_enrg)


def thermal_energy_2d(energy, kb_t):
    return np.exp(-energy / kb_t) / kb_t
