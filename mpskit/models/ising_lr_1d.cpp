#include "ising_lr_1d.hpp"
#include <cmath>

IsingLR1D::IsingLR1D(const json &js)
    : TransverseIsing1D(js), m_alpha(js["alpha"].get<Real>()),
      m_cutoff_radius(jsonGetDefault<Real>(js, "cutoff_radius", -1.0))
{
    m_one_body_terms.clear();
    m_two_body_terms.clear();

    m_one_body_terms.reserve(static_cast<std::size_t>(2 * m_L));
    m_two_body_terms.reserve(static_cast<std::size_t>(std::round(m_L * m_L / 2.0)));

    for (int i = 0; i < m_L; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * m_hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * m_hz, "Sz", i);
    }

    for (int i = 0; i < m_L; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            const auto dist = std::abs(static_cast<Real>(j - i));
            if ((m_cutoff_radius > 0.0) && (dist > m_cutoff_radius))
            {
                continue;
            }
            const auto coupling = -4.0 * m_J / std::pow(dist, m_alpha);
            m_two_body_terms.emplace_back(coupling, "Sz", i, "Sz", j);
        }
    }
}

const Real &IsingLR1D::getAlpha() const
{
    return m_alpha;
}

const Real &IsingLR1D::getCutoffRadius() const
{
    return m_cutoff_radius;
}