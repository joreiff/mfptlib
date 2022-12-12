// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_OBSERVER_HPP
#define MFPTLIB_MATH_OBSERVER_HPP

#include <functional>
#include <utility>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

class Observer
{
public:
    using Function = std::function<void(const VectorsCRef&, double)>;


public:
    explicit Observer(Function func = {}) noexcept
        : func_{std::move(func)}
    {}

    void operator()(const VectorsCRef& states, double t) const
    {
        if(func_)
            func_(states, t);
    }


private:
    Function func_;
};

} // namespace mfptlib

#endif
