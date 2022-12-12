// Copyright 2022 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <mfptlib/math/Propagate.hpp>

#include <type_traits>

#include <mfptlib/core/Errors.hpp>


namespace mfptlib {

namespace detail {

auto partition_record(
    Indices& order, VectorsRef states, const Booleans& predicate
) noexcept -> Index
{
    assert(states.rows() <= order.size());
    assert(states.rows() == predicate.size());

    Index first = 0;
    Index last = predicate.size();

    for(;; ++first)
    {
        for(;; ++first)
        {
            if(first == last)
                return first;
            if(!predicate[first])
                break;
        }

        do
        {
            if(first == --last)
                return first;
        }
        while(!predicate[last]);

        states.row(first).swap(states.row(last));
        std::swap(order[first], order[last]);
    }
}


void restore_order(Indices& order, VectorsRef states) noexcept
{
    static_assert(std::is_signed_v<Index>);
    assert(states.rows() == order.size());

    for(Index cycle = 0; cycle < order.size(); ++cycle)
    {
        Index current = cycle;
        while(order[current] != current)
        {
            const Index target = order[current];
            states.row(cycle).swap(states.row(target));
            order[current] = current;
            current = target;
        }
    }
}

} // namespace detail


auto propagate_to(
    Stepper& stepper, Bath& bath, const System& system,
    VectorsRef states, double t, double t_end,
    const Observer& observe
) -> double
{
    expect(t <= t_end, "Final time t_end must not precede initial time t.");

    observe(states, t);
    while(t < t_end)
    {
        stepper.step(bath, system, states, t);
        observe(states, t);
    }

    return t;
}


auto propagate_while(
    Stepper& stepper, Bath& bath, const System& system,
    VectorsRef states, double t, const Predicate& predicate,
    const Observer& observe
) -> Scalars
{
    VectorsRef all_states{states};
    Indices order = Indices::LinSpaced(states.rows(), 0, states.rows() - 1);
    Scalars t_end{states.rows()};

    observe(states, t);
    while(true)
    {
        const Booleans keep_running = predicate(states, t);
        const Index stop = detail::partition_record(order, states, keep_running);
        t_end(order.segment(stop, states.rows() - stop)) = t;

        if(stop == 0)
            break;
        else if(stop != states.rows())
        {
            stepper.filter_states(keep_running);
            bath.filter_states(keep_running);
        }

        reconstruct(states, all_states.topRows(stop));
        stepper.step(bath, system, states, t);
        observe(states, t);
    }
    detail::restore_order(order, all_states);

    return t_end;
}

} // namespace mfptlib
