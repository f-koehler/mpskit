#include "ising_lr_square.hpp"
#include <cmath>

IsingLRSquare::IsingLRSquare(const json &js)
    : SpinHalfSquare(js["Lx"].get<int>(), js["Ly"].get<int>(), false, false), m_J(js["J"].get<Real>()),
      m_hx(js["hx"].get<Real>()), m_hz(js["hz"].get<Real>()), m_alpha(js["alpha"].get<Real>()),
      m_cutoff_radius(jsonGetDefault<Real>(js, "cutoff_radius", -1.0))
{
    m_name = "IsingLRSquare";

    const auto N = m_Lx * m_Ly;

    m_one_body_terms.reserve(static_cast<std::size_t>(2 * N));
    m_two_body_terms.reserve(static_cast<std::size_t>(std::round(N * N / 2.0)));

    for (int i = 0; i < N; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * m_hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * m_hz, "Sz", i);
    }

    for (int i = 0; i < N; ++i)
    {
        const int x1 = i / m_Ly;
        const int y1 = i % m_Ly;
        for (int j = 0; j < i; ++j)
        {
            const int x2 = j / m_Ly;
            const int y2 = j % m_Ly;
            const auto dx = static_cast<double>(std::abs(x1 - x2));
            const auto dy = static_cast<double>(std::abs(y1 - y2));
            const auto dist = std::sqrt((dx * dx) + (dy * dy));
            if ((m_cutoff_radius > 0.0) && (dist > m_cutoff_radius))
            {
                continue;
            }
            const auto coupling = -4.0 * m_J / std::pow(dist, m_alpha);
            m_two_body_terms.emplace_back(coupling, "Sz", i, "Sz", j);
        }
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

void IsingLRSquare::print(std::ostream &stream) const
{
    SpinHalfSquare::print(stream);
    stream << "\n\tJ:                  " << m_J;
    stream << "\n\thx:                 " << m_hx;
    stream << "\n\thz:                 " << m_hz;
    stream << "\n\talpha:              " << m_alpha;
    stream << "\n\tcutoff radius:      " << m_cutoff_radius;
}