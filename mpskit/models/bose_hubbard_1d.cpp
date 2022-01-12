#include "bose_hubbard_1d.hpp"

#include <algorithm>
#include <cstddef>
#include <itensor/util/iterate.h>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "../json.hpp"
#include "../terms.hpp"
#include "../types.hpp"

BoseHubbard1D::BoseHubbard1D(int L, bool periodic, bool conserve_N, int max_N, const Real &J, const Real &U,
                             const Real &mu)
    : Bosonic1D(L, periodic, conserve_N, max_N), m_J(J), m_U(U), m_mu(mu)
{
    m_one_body_terms.reserve(static_cast<std::size_t>(L));
    m_two_body_terms.reserve(static_cast<std::size_t>(3 * L));

    for (auto i : itensor::range1(L - 1))
    {
        m_two_body_terms.emplace_back(-J, "Adag", i, "A", i + 1);
        m_two_body_terms.emplace_back(-J, "Adag", i + 1, "A", i);
    }
    if (periodic)
    {
        m_two_body_terms.emplace_back(-J, "Adag", 1, "A", L);
        m_two_body_terms.emplace_back(-J, "Adag", L, "A", 1);
    }
    for (auto i : itensor::range1(L))
    {
        m_two_body_terms.emplace_back(U / 2.0, "N", i, "N", i);
        m_one_body_terms.emplace_back(-U / 2.0, "N", i);
    }
}

BoseHubbard1D::BoseHubbard1D(const json &js)
    : BoseHubbard1D(js["L"].get<int>(), js["periodic"].get<bool>(), js["conserve_N"].get<bool>(),
                    js["max_N"].get<int>(), js["J"].get<Real>(), js["U"].get<Real>(), js["mu"].get<Real>())
{
}

auto BoseHubbard1D::getJ() const -> const Real &
{
    return m_J;
}
auto BoseHubbard1D::getU() const -> const Real &
{
    return m_U;
}
auto BoseHubbard1D::getMu() const -> const Real &
{
    return m_mu;
}