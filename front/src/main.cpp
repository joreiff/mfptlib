// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <pybind11/pybind11.h>

#include "math/Bath.hpp"
#include "math/Observer.hpp"
#include "math/Predicate.hpp"
#include "math/Propagate.hpp"
#include "math/Stepper.hpp"
#include "sys/EmptyPlane.hpp"
#include "sys/HarmonicOscillator.hpp"
#include "sys/LithiumCyanide.hpp"
#include "sys/System.hpp"


PYBIND11_MODULE(_backend, m)
{
    mfptlib::class_system(m);
    mfptlib::def_empty_plane(m);
    mfptlib::def_harmonic_oscillator(m);
    mfptlib::def_lithium_cyanide(m);

    mfptlib::class_bath(m);
    mfptlib::def_langevin_bath(m);
    mfptlib::def_exp_memory_bath(m);

    mfptlib::class_stepper(m);
    mfptlib::def_baoab_stepper(m);
    mfptlib::def_fast_baoab_stepper(m);
    mfptlib::def_lf_middle_stepper(m);

    mfptlib::class_observer(m);
    mfptlib::class_predicate(m);
    mfptlib::def_propagate_to(m);
    mfptlib::def_propagate_while(m);
}
