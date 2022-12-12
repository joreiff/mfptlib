#!/usr/bin/env python3
# Copyright 2022 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import numpy as np
import scipy.special

import mfptlib


# Multipole moments <Q_L,0> for the electrostatic energy.
MOMENT_Q = [-1.00, -0.215135, -3.414573, -3.818815, -15.84152, -14.29374, -43.81719]

# Induction energy coefficients C_l1,l2,L.
# Format: (l1, l2): {L: C_l1,l2,L}
# Some parameters include a factor 2 to account for C_l1,l2,L = C_l2,l1,L.
INDUCION_COEFFS = {
    (1, 1): {0: -10.5271, 2: -3.17},
    (2, 1): {1: -20.62328, 3: 3.7320800}, # !!! 4.06 vs 3.7320800
    (2, 2): {0: -57.49396, 2: -106.8192, 4: 17.14139}, # includes (3, 1)
    (3, 2): {1: -202.8972, 3: -75.23207, 5: -28.45514},
    (3, 3): {0: -458.2015, 2: -353.7347, 4: -112.6427, 6: -108.2786},
}

# Damping fit parameters.
DAMPING_R0 = 1.900781
DAMPING_A = 1.515625

# Short-range fit parameters A_L, B_L, and C_L.
SHORT_RNG_PARAMS = [
    # A_L         B_L          C_L
    (-1.3832116, +0.14000706, +0.2078921600),
    (-2.9579132, +1.47977160, -0.0116130820),
    (-4.7420297, +1.81198620, -0.0171806800), # !!! 0.017818 vs 0.0171806800
    (-1.8885299, +1.28750300, +0.0277284910),
    (-4.4143354, +2.32297140, -0.0706927420),
    (-4.0256496, +2.77538320, -0.1377197800),
    (-5.8425899, +3.48085290, -0.1863111400),
    (-2.6168114, +2.65559520, -0.0058815504),
    (-6.3446579, +4.34498010, -0.1529136800),
    (15.2022800, -6.54925370, +1.3025678000),
]


def potential(states):
    theta, r = mfptlib.positions[states].T
    legendre = [legendre_cos(l, theta) for l in range(len(SHORT_RNG_PARAMS))]
    return (
        (pot_electrostat(r, legendre) + pot_induction(r, legendre)) * damping(r)
        + pot_short_rng(r, legendre)
    )


def pot_electrostat(r, legendre):
    return sum(
        r**(-l - 1) * q * legendre[l]
        for l, q in enumerate(MOMENT_Q)
    )


def pot_induction(r, legendre):
    return sum(
        r**(-2 - l1 - l2) * sum(c * legendre[l] for l, c in cl.items())
        for (l1, l2), cl in INDUCION_COEFFS.items()
    )


def pot_short_rng(r, legendre):
    return sum(
        srp * legendre[l]
        for l, srp in enumerate(short_rng_params(r))
    )


def short_rng_params(r):
    for a, b, c in SHORT_RNG_PARAMS:
        yield np.exp(-a - b * r - c * r**2)


def damping(r):
    return 1 - np.exp(-DAMPING_A * (r - DAMPING_R0)**2)


def legendre_cos(l, theta):
    return scipy.special.eval_legendre(l, np.cos(theta))
