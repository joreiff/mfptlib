// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_STEPPER_HPP
#define MFPTLIB_MATH_STEPPER_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include <mfptlib/core/Meta.hpp>
#include <mfptlib/core/Types.hpp>
#include <mfptlib/math/Bath.hpp>
#include <mfptlib/sys/System.hpp>


namespace mfptlib {

class Stepper
{
public:
    template<typename Impl, ProtectSpecial<Impl, Stepper> = true>
    explicit Stepper(Impl&& impl)
        : pimpl_{std::make_unique<Wrapper<std::decay_t<Impl>>>(
            std::forward<Impl>(impl))}
    {}

    void step(Bath& bath, const System& system, VectorsRef states, double& t)
    { pimpl_->step(bath, system, std::move(states), t); }

    void filter_states(const Booleans& predicate)
    { pimpl_->filter_states(predicate); }

    void reset()
    { pimpl_->reset(); }


private:
    struct Interface
    {
        virtual ~Interface() noexcept = default;

        virtual void step(
            Bath& bath, const System& system, VectorsRef states, double& t
        ) = 0;
        virtual void filter_states(const Booleans& predicate) = 0;
        virtual void reset() = 0;
    };

    template<typename Impl>
    class Wrapper final : public Interface
    {
    public:
        template<typename ImplT>
        explicit Wrapper(ImplT&& impl)
            : impl_{std::forward<ImplT>(impl)}
        {}

        void step(
            Bath& bath, const System& system, VectorsRef states, double& t
        ) override
        {
            validate_size(system, states, StateType::Full);
            impl_.step(bath, system, std::move(states), t);
        }

        void filter_states(const Booleans& predicate) override
        {
            if constexpr(requires{ impl_.filter_states(predicate); })
                impl_.filter_states(predicate);
        }

        void reset() override
        {
            if constexpr(requires{ impl_.reset(); })
                impl_.reset();
        }

    private:
        Impl impl_;
    };


private:
    std::unique_ptr<Interface> pimpl_;
};

} // namespace mfptlib

#endif
