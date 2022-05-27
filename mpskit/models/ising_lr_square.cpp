#include "ising_lr_square.hpp"
#include <cmath>

IsingLRSquare::IsingLRSquare(const json &js)
    : SpinHalfSquare(js["Lx"].get<int>(), js["Ly"].get<int>(), false, false), m_J(js["J"].get<Real>()),
      m_hx(js["hx"].get<Real>()), m_hz(js["hz"].get<Real>()), m_alpha(js["alpha"].get<Real>()),
      m_cutoff_radius(jsonGetDefault<Real>(js, "cutoff_radius", -1.0))
{
    const auto N = m_Lx * m_Ly;

    m_one_body_terms.reserve(static_cast<std::size_t>(2 * N));
    m_two_body_terms.reserve(static_cast<std::size_t>(std::round(N * N / 2.0)));

    for (int i = 0; i < N; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * m_hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * m_hz, "Sz", i);
    }

    for (const auto &bond : m_lattice)
    {
        const auto dx = std::abs(bond.x1 - bond.x2);
        const auto dy = std::abs(bond.y1 - bond.y2);
        const auto dist = std::sqrt((dx * dx) + (dy * dy));
        if ((m_cutoff_radius > 0.0) && (dist > m_cutoff_radius))
        {
            continue;
        }
        const auto coupling = -4.0 * m_J / std::pow(dist, m_alpha);
        m_two_body_terms.emplace_back(coupling, "Sz", bond.s1 - 1, "Sz", bond.s2 - 1);
    }
}

const Real &IsingLRSquare::getJ() const
{
    return m_J;
}
const Real &IsingLRSquare::getHx() const
{
    return m_hx;
}
const Real &IsingLRSquare::getHz() const
{
    return m_hz;
}
const Real &IsingLRSquare::getAlpha() const
{
    return m_alpha;
}
const Real &IsingLRSquare::getCutoffRadius() const
{
    return m_cutoff_radius;
}