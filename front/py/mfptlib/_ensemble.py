# Copyright 2022 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import typing

import numpy as np

from . import _backend
from . import _utils


__all__ = [
    'random_gen',
    'maxwell_boltzmann_ensemble',
]


def random_gen(
    rng_or_seed: typing.Union[int, np.random.Generator],
    /,
) -> np.random.Generator:
    """Convert the argument into an instance of :class:`numpy.random.Generator`."""

    if isinstance(rng_or_seed, np.random.Generator):
        return rng_or_seed
    else:
        return np.random.Generator(np.random.PCG64(rng_or_seed))


def maxwell_boltzmann_ensemble(
    system: _backend.System,
    kb_t: float,
    q: np.ndarray,
    rng: typing.Union[int, np.random.Generator],
) -> np.ndarray:
    """
    Generate momenta for a Maxwell–Boltzmann ensemble with temperature *kb_t*.

    :param system: The system to generate the ensemble for.
    :param kb_t: The temperature :math:`k_\\mathrm{B} T`.
    :param q: The position vectors of the ensemble states.
    :param seed: The seed used to initialize the PRNG.
    :returns: An array of :math:`N` states.
    """

    qp = _utils.states(q)
    _utils.momenta[qp] = random_gen(rng).normal(
        scale=np.sqrt(system.masses(qp) * kb_t),
        size=_utils.momenta[qp].shape,
    )
    return qp
