#include "vitagliano_2010.hpp"
#include <cmath>
#include <functional>

namespace Vitagliano2010
{
XXModel::XXModel(int L, const Real &J0, const CouplingFunction &coupling_function, const Real &hx, const Real &hy,
                 const Real &hz)
    : SpinHalf1D(L, false, false, false), m_J0(J0), m_coupling_function(coupling_function), m_hx(hx), m_hy(hy), m_hz(hz)
{
    m_name = "Vitagliano2010XXModel";
}

XXModel::XXModel(const json &js)
    : XXModel(js["L"].get<int>(), jsonGetDefault<Real>(js, "J0", 1.0),
              jsonGetDefault<CouplingFunction>(js, "coupling_function", CouplingFunction::Gaussian),
              jsonGetDefault<Real>(js, "hx", 0.0), jsonGetDefault<Real>(js, "hy", 0.0),
              jsonGetDefault<Real>(js, "hz", 0.0))
{
    m_one_body_terms.reserve(static_cast<std::size_t>(3 * m_L));
    m_two_body_terms.reserve(static_cast<std::size_t>(2 * (m_L - 1)));

    if (m_L % 2)
    {
        throw std::domain_error("L must be even");
    }

    std::function<Real(int)> dist;
    switch (m_coupling_function)
    {
    case Uniform:
        dist = [](int n) {
            (void)n;
            return 1.0;
        };
        break;
    case Exponential:
        dist = [](int n) { return std::exp(-static_cast<Real>(n)); };
        break;
    case Gaussian:
        dist = [](int n) { return std::exp(-2.0 * static_cast<Real>(n * n)); };
        break;
    default:
        throw std::domain_error("invalid coupling distribution");
        break;
    }

    int center = m_L / 2 - 1;

    for (int i = 0; i < m_L - 1; ++i)
    {
        int n = std::abs(center - i);
        m_two_body_terms.emplace_back(2 * m_J0 * dist(n), "Sx", i, "Sx", i + 1);
        m_two_body_terms.emplace_back(2 * m_J0 * dist(n), "Sy", i, "Sy", i + 1);
    }

    for (int i = 0; i < m_L; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * m_hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * m_hy, "Sy", i);
        m_one_body_terms.emplace_back(-2.0 * m_hz, "Sz", i);
    }
}

const Real &XXModel::getJ0() const
{
    return m_J0;
}
const CouplingFunction &XXModel::getCouplingFunction() const
{
    return m_coupling_function;
}
const Real &XXModel::getHx() const
{
    return m_hx;
}
const Real &XXModel::getHy() const
{
    return m_hy;
}
const Real &XXModel::getHz() const
{
    return m_hz;
}

} // namespace Vitagliano2010