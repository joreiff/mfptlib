// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_PREDICATE_HPP
#define MFPTLIB_MATH_PREDICATE_HPP

#include <functional>
#include <utility>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

class Predicate
{
public:
    using Function = std::function<Booleans(const VectorsCRef&, double)>;


public:
    explicit Predicate(Function func)
        : func_{std::move(func)}
    {
        expect(bool{func_},
            "Predicate must be constructed with a non-empty function.");
    }

    auto operator()(const VectorsCRef& states, double t) const -> Booleans
    {
        const Booleans results = func_(states, t);
        expect(results.size() == states.rows(),
            "The size of the predicate results must match the number of states.");
        return results;
    }


private:
    Function func_;
};

} // namespace mfptlib

#endif
