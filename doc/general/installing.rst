..  Copyright 2021 Johannes Reiff
    SPDX-License-Identifier: Apache-2.0

**********
Installing
**********

.. highlight:: console



Basic Instructions
==================

To setup mfptlib on your machine, simply follow these instructions:

1.  (optional) Fork the project through GitHub.

2.  Clone the repository (adapt the URL if forked)::

        $ cd /path/to/src
        $ git clone "git@github.com:joreiff/mfptlib.git"
        $ cd mfptlib

    This will create and enter the directory ``/path/to/src/mfptlib``.

3.  Create and activate a virtual environment::

        $ cd /path/to/venv
        $ python3 -m venv .
        $ . bin/activate

4.  Install mfptlib using ``pip``::

        $ pip install /path/to/src/mfptlib

5.  Install third-party packages like ``numpy``, ``scipy``, and ``matplotlib``.
    Alternatively, you can use the flag ``--system-site-packages``
    when creating the virtual environment.



Development Install
===================

To setup a virtual environment for development, use the :ref:`bin/dev` script.
You might want to run :ref:`bin/bootstrap` beforehand to ensure that
compatible versions of :ref:`bin/dev` dependencies are installed.



Advanced Options
================

The underlying build system is based on `scikit-build`_ and `CMake`_,
see `here <scikit-build cmdline_>`_
for a list of allowed :git-file:`setup.py` options.
To enable easy installation using ``pip``,
arbitrary `CMake`_ variables can be passed to :git-file:`setup.py`
using environment variables of the form ``MFPTLIB_SETUP_<VAR_NAME>``.
Besides standard ones (see `here <CMake variables_>`_ for an extensive list),
builds can be configured using the following variables:

.. cmakevar:: MFPTLIB_COLORED_DIAGNOSTICS

    Due to Ninja's parallel design,
    compilers do not output ANSI color codes by default
    when `CMake`_ is configured using ``-GNinja``.
    Enabling this option forces compilers to output colored warnings and errors.

.. cmakevar:: MFPTLIB_USE_LD

    Use a specific linker (e.g. ``lld``, ``gold``, ``bfd``).

.. cmakevar:: MFPTLIB_USE_LTO

    Whether or not to use link-time optimization
    (a.k.a. interprocedural/whole program optimization).
    Enabled by default.

.. cmakevar:: MFPTLIB_USE_STDLIB

    Use a specific standard library (e.g. ``libc++``).

.. cmakevar:: MFPTLIB_STRIP

    If enabled (the default), strip symbols from the resulting shared library.



Editor Integration
==================

To ensure a consistent editor setup,
a :git-file:`.editorconfig` file is provided,
which is read automatically by many editors.

To integrate mfptlib into your editor,
it is recommended to use the :ref:`bin/dev` script.
For C++ auto-completion,
the compilation database in ``_skbuild/compile_commands.json``
generated during the build can be used.
See :git-file:`mfptlib.sublime-project` for an example setup.


Sublime Text
------------

For convenience,
a simple :git-file:`project file <mfptlib.sublime-project>` is provided.
It features
build systems for the most common development tasks based on `Terminus`_
as well as basic configurations for the `LSP`_ and `TodoReview`_ packages.



.. _scikit-build: https://scikit-build.readthedocs.io
.. _scikit-build cmdline: https://scikit-build.readthedocs.io/en/latest/usage.html#command-line-options
.. _CMake variables: https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html
.. _CMake: https://cmake.org/
.. _LSP: https://lsp.sublimetext.io/
.. _Terminus: https://packagecontrol.io/packages/Terminus
.. _TodoReview: https://packagecontrol.io/packages/TodoReview
