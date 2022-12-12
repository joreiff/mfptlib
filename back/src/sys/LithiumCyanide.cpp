// Copyright 2021 Johannes Reiff
// SPDX-License-Identifier: Apache-2.0

#include <mfptlib/sys/LithiumCyanide.hpp>

#include <array>
#include <cassert>


namespace mfptlib {

namespace {

constexpr double AtomicMass = 1822.888486209; // u / mₑ

constexpr double MassLi = 7.016003437 * AtomicMass;
constexpr double MassC = 12.0 * AtomicMass;
constexpr double MassN = 14.00307400446 * AtomicMass;
constexpr double Mu1 = 1.0 / (1.0 / MassLi + 1.0 / (MassC + MassN));
constexpr double Mu2 = 1.0 / (1.0 / MassC + 1.0 / MassN);
constexpr double DistNC = 2.186;
constexpr double DistNCSquared = DistNC * DistNC;

// Multipole moments <Q_L,0> for the electrostatic energy.
constexpr std::array MomentQ
{
    -1.00, -0.215135, -3.414573, -3.818815, -15.84152, -14.29374, -43.81719,
};

// Induction energy coefficients C_l1,l2,L.
// Format: [l1 + l2 - 2][L]
// Some parameters include a factor 2 to account for C_l1,l2,L = C_l2,l1,L.
constexpr std::array<std::array<double, 7>, 5> InductionCoeffs = {{
    { -10.5271 ,    0.0    ,   -3.17  ,   0.0    ,    0.0    ,   0.0    ,    0.0   },
    {   0.0    ,  -20.62328,    0.0   ,   3.73208,    0.0    ,   0.0    ,    0.0   },
    { -57.49396,    0.0    , -106.8192,   0.0    ,   17.14139,   0.0    ,    0.0   },
    {   0.0    , -202.8972 ,    0.0   , -75.23207,    0.0    , -28.45514,    0.0   },
    {-458.2015 ,    0.0    , -353.7347,   0.0    , -112.6427 ,   0.0    , -108.2786},
}};

// Damping fit parameters.
constexpr double DampingR0 = 1.900781;
constexpr double DampingA = 1.515625;

// Short-range fit parameters A_L, B_L, and C_L.
constexpr std::array<std::array<double, 3>, 10> ShortRngParams
{{
    // A_L        B_L          C_L
    {-1.3832116, +0.14000706, +0.2078921600},
    {-2.9579132, +1.47977160, -0.0116130820},
    {-4.7420297, +1.81198620, -0.0171806800},
    {-1.8885299, +1.28750300, +0.0277284910},
    {-4.4143354, +2.32297140, -0.0706927420},
    {-4.0256496, +2.77538320, -0.1377197800},
    {-5.8425899, +3.48085290, -0.1863111400},
    {-2.6168114, +2.65559520, -0.0058815504},
    {-6.3446579, +4.34498010, -0.1529136800},
    {15.2022800, -6.54925370, +1.3025678000},
}};


enum Column : Index
{
    Theta, R, PTheta, PR,
};


using LegendreCoeffs = Eigen::Array<
    double, Eigen::Dynamic, ShortRngParams.size()>;


// See https://en.wikipedia.org/wiki/Legendre_polynomials#Definition_via_generating_function
auto legendre(const ScalarsCRef& x) noexcept -> LegendreCoeffs
{
    static_assert(ShortRngParams.size() >= 3);

    LegendreCoeffs p{x.size(), ShortRngParams.size()};
    p.col(0) = 1.0;
    p.col(1) = x;
    p.col(2) = 1.5 * x.square() - 0.5;

    constexpr auto to_dbl = [](Eigen::Index i){ return static_cast<double>(i); };
    for(Eigen::Index l = 3; l < p.cols(); ++l)
    {
        p.col(l) =
            to_dbl(2 * l - 1) / to_dbl(l) * x * p.col(l - 1)
            - to_dbl(l - 1) / to_dbl(l) * p.col(l - 2);
    }

    return p;
}


// See https://en.wikipedia.org/wiki/Legendre_polynomials#Recurrence_relations
auto legendre_prime(
    const ScalarsCRef& x, const LegendreCoeffs& p
) noexcept -> LegendreCoeffs
{
    static_assert(ShortRngParams.size() >= 2);

    LegendreCoeffs dp{x.size(), ShortRngParams.size()};
    dp.col(0) = 0.0;
    dp.col(1) = 1.0;
    dp.col(2) = 3.0 * x;

    for(Eigen::Index l = 3; l < dp.cols(); ++l)
        dp.col(l) = l * p.col(l - 1) + x * dp.col(l - 1);

    return dp;
}


void set_pot_long_rng(
    ScalarsRef res, const ScalarsCRef& r, const LegendreCoeffs& p
) noexcept
{
    static_assert(MomentQ.size() == 7);
    static_assert(InductionCoeffs.size() == 5);
    static_assert(LegendreCoeffs::ColsAtCompileTime >= MomentQ.size()
        and LegendreCoeffs::ColsAtCompileTime >= InductionCoeffs[0].size());
    constexpr auto& c = InductionCoeffs;

    const Scalars inv_r = 1.0 / r;
    Scalars inv_r_pow = inv_r;

    res = inv_r_pow * (MomentQ[0] * p.col(0));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (MomentQ[1] * p.col(1));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (MomentQ[2] * p.col(2));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (MomentQ[3] * p.col(3)
        + c[0][0] * p.col(0) + c[0][2] * p.col(2));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (MomentQ[4] * p.col(4)
        + c[1][1] * p.col(1) + c[1][3] * p.col(3));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (MomentQ[5] * p.col(5)
        + c[2][0] * p.col(0) + c[2][2] * p.col(2) + c[2][4] * p.col(4));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (MomentQ[6] * p.col(6)
        + c[3][1] * p.col(1) + c[3][3] * p.col(3) + c[3][5] * p.col(5));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (c[4][0] * p.col(0)
        + c[4][2] * p.col(2) + c[4][4] * p.col(4) + c[4][6] * p.col(6));
}


void add_pot_short_rng(
    Scalars& res, const ScalarsCRef& r, const LegendreCoeffs& p
) noexcept
{
    static_assert(LegendreCoeffs::ColsAtCompileTime >= ShortRngParams.size());

    const Scalars r2 = r.square();
    for(std::size_t i = 0u; i < ShortRngParams.size(); ++i)
    {
        const auto [a, b, c] = ShortRngParams[i];
        res += p.col(static_cast<Index>(i)) * Eigen::exp(-a - b * r - c * r2);
    }
}


void set_force_theta_long_rng(
    ScalarsRef res, const ScalarsCRef& r, const LegendreCoeffs& dp
) noexcept
{
    static_assert(MomentQ.size() == 7);
    static_assert(InductionCoeffs.size() == 5);
    static_assert(LegendreCoeffs::ColsAtCompileTime >= MomentQ.size()
        and LegendreCoeffs::ColsAtCompileTime >= InductionCoeffs[0].size());
    constexpr auto& c = InductionCoeffs;

    const Scalars inv_r = 1.0 / r;
    Scalars inv_r_pow = inv_r;

    res = -inv_r_pow * (MomentQ[0] * dp.col(0));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (MomentQ[1] * dp.col(1));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (MomentQ[2] * dp.col(2));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (MomentQ[3] * dp.col(3)
        + c[0][0] * dp.col(0) + c[0][2] * dp.col(2));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (MomentQ[4] * dp.col(4)
        + c[1][1] * dp.col(1) + c[1][3] * dp.col(3));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (MomentQ[5] * dp.col(5)
        + c[2][0] * dp.col(0) + c[2][2] * dp.col(2) + c[2][4] * dp.col(4));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (MomentQ[6] * dp.col(6)
        + c[3][1] * dp.col(1) + c[3][3] * dp.col(3) + c[3][5] * dp.col(5));
    inv_r_pow *= inv_r;
    res -= inv_r_pow * (c[4][0] * dp.col(0)
        + c[4][2] * dp.col(2) + c[4][4] * dp.col(4) + c[4][6] * dp.col(6));
}


void add_force_theta_short_rng(
    ScalarsRef res, const ScalarsCRef& r, const LegendreCoeffs& dp
) noexcept
{
    static_assert(LegendreCoeffs::ColsAtCompileTime >= ShortRngParams.size());

    const Scalars r2 = r.square();
    for(std::size_t i = 0u; i < ShortRngParams.size(); ++i)
    {
        const auto [a, b, c] = ShortRngParams[i];
        res -= dp.col(static_cast<Index>(i)) * Eigen::exp(-a - b * r - c * r2);
    }
}


auto force_r_long_rng(
    const ScalarsCRef& r, const LegendreCoeffs& p
) noexcept -> Scalars
{
    static_assert(MomentQ.size() == 7);
    static_assert(InductionCoeffs.size() == 5);
    static_assert(LegendreCoeffs::ColsAtCompileTime >= MomentQ.size()
        and LegendreCoeffs::ColsAtCompileTime >= InductionCoeffs[0].size());
    constexpr auto& c = InductionCoeffs;

    const Scalars inv_r = 1.0 / r;
    Scalars inv_r_pow = inv_r * inv_r;

    Scalars res = inv_r_pow * (MomentQ[0] * p.col(0));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (2.0 * MomentQ[1] * p.col(1));
    inv_r_pow *= inv_r;
    res += inv_r_pow * (3.0 * MomentQ[2] * p.col(2));
    inv_r_pow *= inv_r;
    res += 4.0 * inv_r_pow * (MomentQ[3] * p.col(3)
        + c[0][0] * p.col(0) + c[0][2] * p.col(2));
    inv_r_pow *= inv_r;
    res += 5.0 * inv_r_pow * (MomentQ[4] * p.col(4)
        + c[1][1] * p.col(1) + c[1][3] * p.col(3));
    inv_r_pow *= inv_r;
    res += 6.0 * inv_r_pow * (MomentQ[5] * p.col(5)
        + c[2][0] * p.col(0) + c[2][2] * p.col(2) + c[2][4] * p.col(4));
    inv_r_pow *= inv_r;
    res += 7.0 * inv_r_pow * (MomentQ[6] * p.col(6)
        + c[3][1] * p.col(1) + c[3][3] * p.col(3) + c[3][5] * p.col(5));
    inv_r_pow *= inv_r;
    res += 8.0 * inv_r_pow * (c[4][0] * p.col(0)
        + c[4][2] * p.col(2) + c[4][4] * p.col(4) + c[4][6] * p.col(6));

    return res;
}


void add_force_r_short_rng(
    ScalarsRef res, const ScalarsCRef& r, const LegendreCoeffs& p
) noexcept
{
    static_assert(LegendreCoeffs::ColsAtCompileTime >= ShortRngParams.size());

    const Scalars r2 = r.square();
    for(std::size_t i = 0u; i < ShortRngParams.size(); ++i)
    {
        const auto [a, b, c] = ShortRngParams[i];
        res += p.col(static_cast<Index>(i)) * (b + 2 * c * r)
            * Eigen::exp(-a - b * r - c * r2);
    }
}


auto damping(const ScalarsCRef& r) noexcept
{
    return 1.0 - Eigen::exp(-DampingA * (r - DampingR0).square());
}


auto ddamping_dr(const ScalarsCRef& r) noexcept
{
    return 2.0 * DampingA * (r - DampingR0)
        * Eigen::exp(-DampingA * (r - DampingR0).square());
}

} // namespace


auto potential(
    [[maybe_unused]] const LithiumCyanide& model,
    const VectorsCRef& states,
    [[maybe_unused]] double t
) noexcept -> Scalars
{
    assert(states.cols() == 2 * degrees_of_freedom(model));

    const LegendreCoeffs p_cos = legendre(Eigen::cos(states.col(Theta)).eval());
    Scalars res{states.rows()};

    set_pot_long_rng(res, states.col(R), p_cos);
    res *= damping(states.col(R));
    add_pot_short_rng(res, states.col(R), p_cos);

    return res;
}


auto force(
    [[maybe_unused]] const LithiumCyanide& model,
    const VectorsCRef& states,
    [[maybe_unused]] double t
) noexcept -> Vectors
{
    assert(states.cols() == 2 * degrees_of_freedom(model));

    const Scalars cos_theta = Eigen::cos(states.col(Theta));
    const LegendreCoeffs p_cos = legendre(cos_theta);
    const LegendreCoeffs dp_cos =
        legendre_prime(cos_theta, p_cos).colwise()
        * -Eigen::sin(states.col(Theta));

    Vectors res{states.rows(), degrees_of_freedom(model)};

    set_force_theta_long_rng(res.col(Theta), states.col(R), dp_cos);
    res.col(Theta) *= damping(states.col(R));
    add_force_theta_short_rng(res.col(Theta), states.col(R), dp_cos);

    set_pot_long_rng(res.col(R), states.col(R), p_cos);
    res.col(R) *= -ddamping_dr(states.col(R));
    res.col(R) += force_r_long_rng(states.col(R), p_cos) * damping(states.col(R));
    add_force_r_short_rng(res.col(R), states.col(R), p_cos);
    res.col(R) += states.col(PTheta).square() / (Mu1 * states.col(R).cube());

    return res;
}


auto masses(
    [[maybe_unused]] const LithiumCyanide& model, const VectorsCRef& states
) noexcept -> Vectors
{
    assert(states.cols() == 2 * degrees_of_freedom(model));

    return (Vectors{states.rows(), 2} <<
        1.0 / (1.0 / (Mu1 * states.col(R).square()) + 1.0 / (Mu2 * DistNCSquared)),
        Scalars::Constant(states.rows(), Mu1)
    ).finished();
}

} // namespace mfptlib
