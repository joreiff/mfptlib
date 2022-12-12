..  Copyright 2021 Johannes Reiff
    SPDX-License-Identifier: Apache-2.0

.. _bin:

*****
Tools
*****

.. highlight:: console

mfptlib provides some scripts in it's :git-file:`bin/` directory.
They are mostly used to automate certain tasks
around building and testing the software package.



.. _bin/bootstrap:

:git-file:`bin/bootstrap`
=========================

.. program:: bin/bootstrap

Simple shell script to install dependencies of :ref:`bin/dev`. ::

    $ bin/bootstrap [PYTHON [PIP_OPTIONS]...]

.. option:: PYTHON

    The Python executable to use.
    Defaults to ``python3``.

.. option:: PIP_OPTIONS

    Additional options to pass to ``pip``.



.. _bin/dev:

:git-file:`bin/dev`
===================

.. program:: bin/dev

Helper script for managing the development virtual environment. ::

    $ bin/dev init-venv [--config CONFIG] [CMAKE_VARS]...
    $ bin/dev install [--config CONFIG] [CMAKE_VARS]...
    $ bin/dev build [CMAKE_VARS]...
    $ bin/dev test [PYTEST_ARGS]...
    $ bin/dev docs [BUILDER]
    $ bin/dev clean
    $ bin/dev distclean
    $ bin/dev run [CMD_LINE]...
    $ bin/dev wheel [--config CONFIG] [--subdir SUBDIR] [CMAKE_VARS]...

.. option:: init-venv [--config CONFIG] [CMAKE_VARS]...

    Initialize the development virtual environment
    and install an editable version of mfptlib into it.
    CMake variables can be read from the TOML file ``CONFIG``
    or specified directly on the command line with a ``NAME=VALUE`` syntax.
    Append a section name in square brackets to the TOML path
    (i.e. ``PATH[TABLE]``)
    to overwrite definitions from the root table.

.. option:: install [--config CONFIG] [CMAKE_VARS]...

    (Re-)Install mfptlib into the virtual environment.
    CMake variables can be specified as described in :option:`init-venv`.

.. option:: build [CMAKE_VARS]...

    Compile mfptlib and run C++ unit tests.
    CMake variables can be specified with a ``NAME=VALUE`` syntax.

.. option:: test [PYTEST_ARGS]...

    Run the `pytest`_ test suite on the development version of mfptlib.
    Extra arguments are passed to `pytest`_.

.. option:: docs [BUILDER]

    Build the documentation.
    Common builders are ``html`` and ``latexpdf``.

.. option:: clean

    Remove all temporary build artifacts
    generated for the editable development install
    including the virtual environment.

.. option:: distclean

    Like :option:`clean`, but also remove built wheels and documentation.

.. option:: run [CMD_LINE]...

    Run a command in the development virtual environment.
    Defaults to an interactive Bash shell.

.. option:: wheel [--config CONFIG] [--subdir SUBDIR] [CMAKE_VARS]...

    Build a wheel in a subdirectory of ``_dist/``.
    CMake variables can be specified as described in :option:`init-venv`.



.. _pytest: https://docs.pytest.org/
