#include "xy_inhom.hpp"
#include <cmath>

namespace XYInhom
{
XYInhom::XYInhom(int L, const Real &J0, const CouplingDistribution &coupling_distribution, const Real &alpha,
                 const Real &hx, const Real &hy, const Real &hz)
    : SpinHalf1D(L, false, false, false), m_J0(J0), m_coupling_distribution(coupling_distribution), m_alpha(alpha),
      m_hx(hx), m_hy(hy), m_hz(hz)
{
    m_name = "XYInhom";
    if (m_L % 2)
    {
        throw std::domain_error("L must be even");
    }

    std::function<Real(int)> dist;
    switch (m_coupling_distribution)
    {
    case Uniform:
        dist = [](int r) {
            (void)r;
            return 1.0;
        };
        break;
    case Exponential:
        dist = [](int r) { return std::exp(-static_cast<Real>(r - 1)); };
        break;
    case Gaussian:
        dist = [](int r) { return std::exp(-static_cast<Real>(2 * (r - 1) * (r - 1))); };
        break;
    case Logarithmic:
        dist = [](int r) { return 1.0 / std::log2(static_cast<Real>(r + 1)); };
        break;
    default:
        throw std::domain_error("invalid coupling distribution");
        break;
    }

    for (int i = 0; i < m_L; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            const auto r = std::abs(i - j);
            const auto J = m_J0 * dist(r) / std::pow(static_cast<Real>(r), m_alpha);
            m_two_body_terms.emplace_back(4.0 * J, "S+", i, "S-", j);
            m_two_body_terms.emplace_back(4.0 * J, "S-", i, "S+", j);
        }
    }

    for (int i = 0; i < m_L; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * m_hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * m_hy, "Sy", i);
        m_one_body_terms.emplace_back(-2.0 * m_hz, "Sz", i);
    }
}

XYInhom::XYInhom(const json &js)
    : XYInhom(js.at("L").get<int>(), jsonGetDefault<Real>(js, "J0", 1.0),
              jsonGetDefault<CouplingDistribution>(js, "coupling_distribution", CouplingDistribution::Uniform),
              jsonGetDefault<Real>(js, "alpha", 0.0), jsonGetDefault<Real>(js, "hx", 0.0),
              jsonGetDefault<Real>(js, "hy", 0.0), jsonGetDefault<Real>(js, "hz", 0.0))
{
}

const Real &XYInhom::getJ0() const
{
    return m_J0;
}
const CouplingDistribution &XYInhom::getCouplingDistribution() const
{
    return m_coupling_distribution;
}
const Real &XYInhom::getAlpha() const
{
    return m_alpha;
}
const Real &XYInhom::getHx() const
{
    return m_hx;
}
const Real &XYInhom::getHy() const
{
    return m_hy;
}
const Real &XYInhom::getHz() const
{
    return m_hz;
}

} // namespace XYInhom