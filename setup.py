#!/usr/bin/env python3
# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

import importlib
import inspect
import os
import pathlib
import platform
import re
import subprocess
import sys

import setuptools
import setuptools_scm.version
import skbuild
import skbuild.constants


def main():
    skbuild.setup(
        packages=setuptools.find_packages('front/py'),
        package_dir={'': 'front/py'},
        use_scm_version=scm_settings,
        cmake_languages=('CXX',),
        cmake_args=list(cmake_args()),
        cmdclass={'build_ext': BuildExt},
    )


def scm_settings():
    def local_scheme(version):
        original = setuptools_scm.version.get_local_node_and_date(version)
        return f'{original}.{host_name()}' if original else f'+{host_name()}'

    return {
        'local_scheme': local_scheme,
        'write_to': 'front/py/mfptlib/_version.py',
        'write_to_template': inspect.cleandoc('''
            # Copyright 2021 Johannes Reiff
            # SPDX-License-Identifier: Apache-2.0
            # pylint: disable=unexpected-line-ending-format

            VERSION = '{version}'
        ''') + '\n',
    }


class BuildExt(skbuild.command.build_ext.build_ext):
    def run(self):
        super().run()
        self.__gen_compile_db()
        self.__run_unit_tests()

    @staticmethod
    def __gen_compile_db():
        compdb_src = CMAKE_BUILD_DIR / 'compile_commands.json'
        if has_module('compdb') and compdb_src.is_file():
            print('Generating enhanced compilation database')
            with COMPDB_OUT.open('wb') as fp:
                subprocess.run([
                    sys.executable, '-m', 'compdb',
                    '-p', str(CMAKE_BUILD_DIR), 'list',
                ], stdout=fp, check=True)

    @staticmethod
    def __run_unit_tests():
        print('Running C++ unit tests')
        subprocess.run(
            [str(CMAKE_BUILD_DIR / 'back' / 'mfptlib-back-test')],
            check=True,
        )


def has_module(name):
    return importlib.util.find_spec(name) is not None


def cmake_args():
    for key, val in os.environ.items():
        if key.startswith(ENV_VAR_PREFIX):
            yield '-D{}={}'.format(key[len(ENV_VAR_PREFIX):], val)


def host_name():
    node_name = platform.node().split('.', 1)[0]
    return re.sub(r'[^0-9A-Za-z]', '', node_name) or 'unknown'


skbuild.constants.set_skbuild_plat_name(f'build-{host_name()}')
PROJECT_ROOT = pathlib.Path(__file__).resolve().parent
CMAKE_BUILD_DIR = PROJECT_ROOT / skbuild.constants.CMAKE_BUILD_DIR()
COMPDB_OUT = PROJECT_ROOT / '_skbuild' / 'compile_commands.json'
ENV_VAR_PREFIX = 'MFPTLIB_SETUP_'


if __name__ == '__main__':
    main()
