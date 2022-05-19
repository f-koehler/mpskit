#include "ising_lr_1d.hpp"
#include <cmath>

SpinHalfLR1D::SpinHalfLR1D(const json &js)
    : TransverseIsing1D(js), m_alpha(js["alpha"].get<Real>()), m_cutoff(jsonGetDefault<Real>(js, "cutoff", -1.0))
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
            const auto coupling = -m_J / std::pow(std::abs(static_cast<Real>(j - i)), m_alpha);
            if (coupling > m_cutoff)
            {
                continue;
            }
            m_two_body_terms.emplace_back(coupling, "Sz", i, "Sz", j);
        }
    }
}

const Real &SpinHalfLR1D::getAlpha() const
{
    return m_alpha;
}

const Real &SpinHalfLR1D::getCutoff() const
{
    return m_cutoff;
}