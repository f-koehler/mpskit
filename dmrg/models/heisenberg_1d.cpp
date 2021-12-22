#include "heisenberg_1d.hpp"

#include <itensor/util/iterate.h>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cstddef>
#include <map>
#include <vector>

#include "/home/fkoehler/code/ising/dependencies/dmrg/dmrg/json.hpp"
#include "/home/fkoehler/code/ising/dependencies/dmrg/dmrg/terms.hpp"
#include "/home/fkoehler/code/ising/dependencies/dmrg/dmrg/types.hpp"

Heisenberg1D::Heisenberg1D(int L, bool periodic, const Real &Jx, const Real &Jy, const Real &Jz, const Real &hx,
                           const Real &hy, const Real &hz)
    : SpinHalf1D(L, periodic, false, false), m_Jx(Jx), m_Jy(Jy), m_Jz(Jz), m_hx(hx), m_hy(hy), m_hz(hz)
{
    m_one_body_terms.reserve(static_cast<std::size_t>(3 * L));
    m_two_body_terms.reserve(static_cast<std::size_t>(3 * L));

    for (auto i : itensor::range1(L))
    {
        m_one_body_terms.push_back({-2.0 * hx, "Sx", i});
        m_one_body_terms.push_back({-2.0 * hy, "Sy", i});
        m_one_body_terms.push_back({-2.0 * hz, "Sz", i});
    }

    for (auto i : itensor::range1(L - 1))
    {
        m_two_body_terms.push_back({-4.0 * Jx, "Sx", i, "Sx", i + 1});
        m_two_body_terms.push_back({-4.0 * Jy, "Sy", i, "Sy", i + 1});
        m_two_body_terms.push_back({-4.0 * Jz, "Sz", i, "Sz", i + 1});
    }
    if (periodic)
    {
        m_two_body_terms.push_back({-4.0 * Jx, "Sx", 1, "Sx", L});
        m_two_body_terms.push_back({-4.0 * Jy, "Sy", 1, "Sy", L});
        m_two_body_terms.push_back({-4.0 * Jz, "Sz", 1, "Sz", L});
    }
}

Heisenberg1D::Heisenberg1D(const json &js)
    : Heisenberg1D(js["L"].get<int>(), js["periodic"].get<bool>(), js["Jx"].get<Real>(), js["Jy"].get<Real>(),
                   js["Jz"].get<Real>(), js["hx"].get<Real>(), js["hy"].get<Real>(), js["hz"].get<Real>())
{
}

auto Heisenberg1D::getJx() const -> const Real &
{
    return m_Jx;
}
auto Heisenberg1D::getJy() const -> const Real &
{
    return m_Jy;
}
auto Heisenberg1D::getJz() const -> const Real &
{
    return m_Jz;
}
auto Heisenberg1D::getHx() const -> const Real &
{
    return m_hx;
}
auto Heisenberg1D::getHy() const -> const Real &
{
    return m_hy;
}
auto Heisenberg1D::getHz() const -> const Real &
{
    return m_hz;
}