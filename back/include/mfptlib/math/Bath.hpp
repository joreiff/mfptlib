// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_MATH_BATH_HPP
#define MFPTLIB_MATH_BATH_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Meta.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

class Bath
{
public:
    template<typename Impl, ProtectSpecial<Impl, Bath> = true>
    explicit Bath(Impl&& impl)
        : pimpl_{std::make_unique<Wrapper<std::decay_t<Impl>>>(
            std::forward<Impl>(impl))}
    {}

    void apply_forces(VectorsRef momenta, const VectorsCRef& masses, double dt)
    { pimpl_->apply_forces(std::move(momenta), masses, dt); }

    void filter_states(const Booleans& predicate)
    { pimpl_->filter_states(predicate); }

    void reset()
    { pimpl_->reset(); }


private:
    struct Interface
    {
        virtual ~Interface() noexcept = default;

        virtual void apply_forces(
            VectorsRef momenta, const VectorsCRef& masses, double dt) = 0;
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

        void apply_forces(
            VectorsRef momenta, const VectorsCRef& masses, double dt
        ) override
        {
            expect(
                momenta.rows() == masses.rows()
                    and momenta.cols() == masses.cols(),
                "Sizes of momenta and masses must match.");
            impl_.apply_forces(std::move(momenta), masses, dt);
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
