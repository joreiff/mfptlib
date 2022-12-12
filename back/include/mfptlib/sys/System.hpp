// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#pragma once
#ifndef MFPTLIB_SYS_SYSTEM_HPP
#define MFPTLIB_SYS_SYSTEM_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include <mfptlib/core/Errors.hpp>
#include <mfptlib/core/Meta.hpp>
#include <mfptlib/core/Types.hpp>


namespace mfptlib {

enum class StateType
{
    Full, Positions, Momenta,
};


template<typename System>
void validate_size(
    const System& sys, const VectorsCRef& states, StateType type
)
{
    Index multiplier = (type == StateType::Full) ? 2 : 1;
    expect(states.cols() == multiplier * degrees_of_freedom(sys),
        "Size of the passed states is incompatible with the system.");
}


class System
{
public:
    template<typename Impl, ProtectSpecial<Impl, System> = true>
    explicit System(Impl&& impl)
        : pimpl_{std::make_unique<Wrapper<std::decay_t<Impl>>>(
            std::forward<Impl>(impl))}
    {}

    [[nodiscard]]
    friend auto potential(
        const System& sys, const VectorsCRef& states, double t
    ) -> Scalars
    { return sys.pimpl_->do_potential(states, t); }

    [[nodiscard]]
    friend auto force(
        const System& sys, const VectorsCRef& states, double t
    ) -> Vectors
    { return sys.pimpl_->do_force(states, t); }

    [[nodiscard]]
    friend auto masses(
        const System& sys, const VectorsCRef& states
    ) -> Vectors
    { return sys.pimpl_->do_masses(states); }

    friend auto degrees_of_freedom(const System& sys) -> Index
    { return sys.pimpl_->do_degrees_of_freedom(); }


private:
    struct Interface
    {
        virtual ~Interface() noexcept = default;

        virtual auto do_potential(
            const VectorsCRef& states, double t) const -> Scalars = 0;
        virtual auto do_force(
            const VectorsCRef& states, double t) const -> Vectors = 0;
        virtual auto do_masses(
            const VectorsCRef& states) const -> Vectors = 0;
        virtual auto do_degrees_of_freedom() const -> Index = 0;
    };

    template<typename Impl>
    class Wrapper final : public Interface
    {
    public:
        template<typename ImplT>
        Wrapper(ImplT&& impl)
            : impl_{std::forward<ImplT>(impl)}
        {}

        auto do_potential(
            const VectorsCRef& states, double t
        ) const -> Scalars override
        {
            validate_size(impl_, states, StateType::Full);
            return potential(impl_, states, t);
        }

        auto do_force(
            const VectorsCRef& states, double t
        ) const -> Vectors override
        {
            validate_size(impl_, states, StateType::Full);
            return force(impl_, states, t);
        }

        auto do_masses(
            const VectorsCRef& states
        ) const -> Vectors override
        {
            validate_size(impl_, states, StateType::Full);
            return masses(impl_, states);
        }

        auto do_degrees_of_freedom() const -> Index override
        { return degrees_of_freedom(impl_); }

    private:
        Impl impl_;
    };


private:
    std::unique_ptr<Interface> pimpl_;
};


[[nodiscard]]
inline auto kinetic_energy(
    const System& sys, const VectorsCRef& states
) -> Scalars
{
    return 0.5 * (momenta(states).square() / masses(sys, states)).rowwise().sum();
}


[[nodiscard]]
inline auto total_energy(
    const System& sys, const VectorsCRef& states, double t
) -> Scalars
{
    return potential(sys, states, t) + kinetic_energy(sys, states);
}

} // namespace mfptlib

#endif
