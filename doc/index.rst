..  Copyright 2021 Johannes Reiff
    SPDX-License-Identifier: Apache-2.0

*******
mfptlib
*******

Release v\ |release|.

Welcome to the official documentation of *mfptlib*,
a software package for the calculation of mean first-passage times
using (generalized) Langevin dynamics.
mfptlib aims to be usable by average scientists without advanced coding skills
while still being fast.
To achieve this goal,
mfptlib is implemented as a Python package backed by a C++ module.

This project currently lacks
extensive user documentation beyond an API reference.
Interested users may instead want to have a look at
the frontend test suite located in :git-file:`front/test/`
or the sample scripts in :git-file:`example/`.


.. toctree::
    :caption: Documentation
    :maxdepth: 2

    general/installing
    general/troubleshooting

.. toctree::
    :caption: Reference
    :maxdepth: 2

    reference/bin
    reference/py
    reference/bibliography
