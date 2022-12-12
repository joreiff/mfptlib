// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_CORE_CACHE_HPP
#define MFPTLIB_CORE_CACHE_HPP

#include <utility>

#include <Eigen/Dense>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

template<typename Array>
class Cache
{
public:
    explicit Cache() noexcept = default;

    Cache(const Cache& rhs) noexcept = default;
    Cache(Cache&& rhs) noexcept = default;

    auto operator=(const Cache& rhs) noexcept -> Cache&
    { return *this = *rhs; }

    auto operator=(Cache&& rhs) noexcept -> Cache& = default;

    template<typename Derived>
    auto operator=(const Eigen::EigenBase<Derived>& rhs) noexcept -> Cache&
    {
        rows_ = rhs.rows();
        if(rhs.cols() == array_.cols() and rhs.rows() <= array_.cols())
            **this = rhs;
        else
            array_ = rhs;

        return *this;
    }

    auto operator=(Array&& rhs) noexcept -> Cache&
    {
        array_ = std::move(rhs);
        rows_ = array_.rows();
        return *this;
    }


    auto operator*() noexcept
    { return array_.topRows(rows_); }

    auto operator*() const noexcept
    { return array_.topRows(rows_); }

    auto rows() const noexcept -> Index
    { return rows_; }

    auto cols() const noexcept -> Index
    { return array_.cols(); }

    auto size() const noexcept -> Index
    { return rows() * cols(); }


    void filter_rows(const Booleans& predicate)
    {
        expect(rows_ == predicate.size(),
            "The predicate size must match the number of rows.");

        auto block = **this;
        Index write{0};

        for(;; ++write)
        {
            for(;; ++write)
            {
                if(write == rows_)
                    return;
                if(!predicate[write])
                    break;
            }

            do
            {
                if(write == --rows_)
                    return;
            }
            while(!predicate[rows_]);

            block.row(write) = block.row(rows_);
        }
    }

    void reset()
    { *this = Array{}; }


private:
    Array array_{};
    Index rows_{0};
};

} // namespace mfptlib

#endif
