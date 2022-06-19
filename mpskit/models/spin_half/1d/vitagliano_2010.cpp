#include "vitagliano_2010.hpp"
#include <cmath>
#include <functional>

namespace Vitagliano2010
{
XXModel::XXModel(int L, const Real &J0, const CouplingDistribution &coupling_distribution)
    : SpinHalf1D(L, false, false, false), m_J0(J0), m_coupling_distribution(coupling_distribution)
{
    m_name = "Vitagliano2010XXModel";
}

XXModel::XXModel(const json &js)
    : XXModel(js["L"].get<int>(), jsonGetDefault<Real>(js, "J0", 1.0),
              jsonGetDefault<CouplingDistribution>(js, "coupling_distribution", CouplingDistribution::GaussianDecay))
{
    m_two_body_terms.reserve(static_cast<std::size_t>(2 * (m_L - 1)));

    if (m_L % 2)
    {
        throw std::domain_error("L must be even");
    }

    std::function<Real(int)> dist;
    switch (m_coupling_distribution)
    {
    case Uniform:
        dist = [](int n) {
            (void)n;
            return 1.0;
        };
        break;
    case ExponentialDecay:
        dist = [](int n) { return std::exp(-static_cast<Real>(n)); };
        break;
    case GaussianDecay:
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
        m_two_body_terms.emplace_back(-2 * m_J0 * dist(n), "Sx", i, "Sx", i + 1);
        m_two_body_terms.emplace_back(-2 * m_J0 * dist(n), "Sy", i, "Sy", i + 1);
    }
}

} // namespace Vitagliano2010