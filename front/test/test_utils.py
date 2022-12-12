# Copyright 2022 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import numpy as np

import mfptlib


STATES_TIME = np.array([
    [0.0, 0.1, 0.2, 0.3, 0.4],
    [1.0, 1.1, 1.2, 1.3, 1.4],
    [2.0, 2.1, 2.2, 2.3, 2.4],
    [3.0, 3.1, 3.2, 3.3, 3.4],
    [4.0, 4.1, 4.2, 4.3, 4.4],
    [5.0, 5.1, 5.2, 5.3, 5.4],
])
STATES = np.array([
    [0.0, 0.1, 0.2, 0.3],
    [1.0, 1.1, 1.2, 1.3],
    [2.0, 2.1, 2.2, 2.3],
    [3.0, 3.1, 3.2, 3.3],
    [4.0, 4.1, 4.2, 4.3],
    [5.0, 5.1, 5.2, 5.3],
])
POSITIONS = np.array([
    [0.0, 0.1],
    [1.0, 1.1],
    [2.0, 2.1],
    [3.0, 3.1],
    [4.0, 4.1],
    [5.0, 5.1],
])
MOMENTA = np.array([
    [0.2, 0.3],
    [1.2, 1.3],
    [2.2, 2.3],
    [3.2, 3.3],
    [4.2, 4.3],
    [5.2, 5.3],
])
TIME = np.array([
    0.4,
    1.4,
    2.4,
    3.4,
    4.4,
    5.4,
])


def test_dofs():
    assert mfptlib.dofs(STATES_TIME) == 2
    assert mfptlib.dofs(STATES) == 2

    assert mfptlib.dofs(STATES_TIME[0]) == 2
    assert mfptlib.dofs(STATES[0]) == 2


def test_time():
    assert np.all(mfptlib.time[STATES_TIME] == TIME)
    assert mfptlib.time[STATES] is None

    assert mfptlib.time[STATES_TIME[0]] == TIME[0]
    assert mfptlib.time[STATES[0]] is None


def test_positions():
    assert np.all(mfptlib.positions[STATES_TIME] == POSITIONS)
    assert np.all(mfptlib.positions[STATES] == POSITIONS)

    assert np.all(mfptlib.positions[STATES_TIME[0]] == POSITIONS[0])
    assert np.all(mfptlib.positions[STATES[0]] == POSITIONS[0])

    for idx in range(2):
        assert np.all(mfptlib.positions[STATES_TIME][..., idx] == POSITIONS[:, idx])
        assert np.all(mfptlib.positions[STATES][..., idx] == POSITIONS[:, idx])

        assert np.all(mfptlib.positions[STATES_TIME[0]][..., idx] == POSITIONS[0, idx])
        assert np.all(mfptlib.positions[STATES[0]][..., idx] == POSITIONS[0, idx])


def test_momenta():
    assert np.all(mfptlib.momenta[STATES_TIME] == MOMENTA)
    assert np.all(mfptlib.momenta[STATES] == MOMENTA)

    assert np.all(mfptlib.momenta[STATES_TIME[0]] == MOMENTA[0])
    assert np.all(mfptlib.momenta[STATES[0]] == MOMENTA[0])

    for idx in range(2):
        assert np.all(mfptlib.momenta[STATES_TIME][..., idx] == MOMENTA[:, idx])
        assert np.all(mfptlib.momenta[STATES][..., idx] == MOMENTA[:, idx])

        assert np.all(mfptlib.momenta[STATES_TIME[0]][..., idx] == MOMENTA[0, idx])
        assert np.all(mfptlib.momenta[STATES[0]][..., idx] == MOMENTA[0, idx])


def test_states():
    zero_p = np.zeros_like(MOMENTA)
    zero_t = np.zeros_like(TIME).reshape((-1, 1))
    states_zero_p = np.concatenate((POSITIONS, zero_p), axis=1)
    states_zero_pt = np.concatenate((POSITIONS, zero_p, zero_t), axis=1)
    states_zero_t = np.concatenate((STATES, zero_t), axis=1)

    assert np.all(mfptlib.states(POSITIONS) == states_zero_p)
    assert np.all(mfptlib.states(POSITIONS, t=0.0) == states_zero_pt)
    assert np.all(mfptlib.states(POSITIONS, MOMENTA) == STATES)
    assert np.all(mfptlib.states(POSITIONS, MOMENTA, t=0.0) == states_zero_t)
    assert np.all(mfptlib.states(POSITIONS, MOMENTA, TIME) == STATES_TIME)

    assert np.all(mfptlib.states(POSITIONS[0]) == states_zero_p[0])
    assert np.all(mfptlib.states(POSITIONS[0], t=0.0) == states_zero_pt[0])
    assert np.all(mfptlib.states(POSITIONS[0], MOMENTA[0]) == STATES[0])
    assert np.all(mfptlib.states(POSITIONS[0], MOMENTA[0], t=0.0) == states_zero_t[0])
    assert np.all(mfptlib.states(POSITIONS[0], MOMENTA[0], TIME[0]) == STATES_TIME[0])


def test_grid():
    assert np.all(mfptlib.grid([1.0, 2.0, 3.0], [1.1, 2.1, 3.1]) == [
        [1.0, 1.1], [1.0, 2.1], [1.0, 3.1],
        [2.0, 1.1], [2.0, 2.1], [2.0, 3.1],
        [3.0, 1.1], [3.0, 2.1], [3.0, 3.1],
    ])
