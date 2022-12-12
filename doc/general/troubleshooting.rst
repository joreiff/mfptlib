..  Copyright 2021 Johannes Reiff
    SPDX-License-Identifier: Apache-2.0

***************
Troubleshooting
***************

In the following sections you can find
possible solutions to common problems concerning mfptlib.
The maintainers are very grateful for
you looking here first before bugging them.



Illegal Instructions
====================

The build system uses the compiler option ``-march=native``
to optimize for the hardware architecture of the machine you are building on
(e.g. by using SIMD and AVX instructions).
While this can significantly improve performance,
it also means that a specific build may not run on other machines.
Therefore, if you encounter an error
stating that an illegal instruction has been encountered,
please make sure that you compile on the oldest machine
the specific binary should run on.
This is especially important for workload management systems like `HTCondor`_.



.. _HTCondor: https://research.cs.wisc.edu/htcondor/
