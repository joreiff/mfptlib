# Copyright 2021 Johannes Reiff
# SPDX-License-Identifier: Apache-2.0

target_sources(mfptlib-glue PRIVATE
    math/Bath.cpp
    math/Bath.hpp
    math/Observer.cpp
    math/Observer.hpp
    math/Predicate.cpp
    math/Predicate.hpp
    math/Propagate.cpp
    math/Propagate.hpp
    math/Stepper.cpp
    math/Stepper.hpp
    sys/EmptyPlane.cpp
    sys/EmptyPlane.hpp
    sys/HarmonicOscillator.cpp
    sys/HarmonicOscillator.hpp
    sys/LithiumCyanide.cpp
    sys/LithiumCyanide.hpp
    sys/System.cpp
    sys/System.hpp
    main.cpp
)
