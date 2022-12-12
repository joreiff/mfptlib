// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_CORE_ERRORS_HPP
#define MFPTLIB_CORE_ERRORS_HPP

#include <stdexcept>
#include <utility>


namespace mfptlib {

template<typename Exception = std::invalid_argument, typename... Args>
void expect(bool condition, Args&&... args)
{
    if(!condition) [[unlikely]]
        throw Exception(std::forward<Args>(args)...);
}

} // namespace mfptlib

#endif
