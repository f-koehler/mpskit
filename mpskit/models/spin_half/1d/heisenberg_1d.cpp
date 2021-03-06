#include "heisenberg_1d.hpp"

#include <algorithm>
#include <cstddef>
#include <itensor/util/iterate.h>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "../../../json.hpp"
#include "../../../terms.hpp"
#include "../../../types.hpp"

Heisenberg1D::Heisenberg1D(int L, bool periodic, const Real &Jx, const Real &Jy, const Real &Jz, const Real &hx,
                           const Real &hy, const Real &hz)
    : SpinHalf1D(L, periodic, false, false), m_Jx(Jx), m_Jy(Jy), m_Jz(Jz), m_hx(hx), m_hy(hy), m_hz(hz)
{
    m_name = "Heisenberg1D";

    m_one_body_terms.reserve(static_cast<std::size_t>(3 * L));
    m_two_body_terms.reserve(static_cast<std::size_t>(3 * L));

    for (int i = 0; i < L; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * hy, "Sy", i);
        m_one_body_terms.emplace_back(-2.0 * hz, "Sz", i);
    }

    for (int i = 0; i < L - 1; ++i)
    {
        m_two_body_terms.emplace_back(-4.0 * Jx, "Sx", i, "Sx", i + 1);
        m_two_body_terms.emplace_back(-4.0 * Jy, "Sy", i, "Sy", i + 1);
        m_two_body_terms.emplace_back(-4.0 * Jz, "Sz", i, "Sz", i + 1);
    }

    if (periodic)
    {
        m_two_body_terms.emplace_back(-4.0 * Jx, "Sx", L - 1, "Sx", 0);
        m_two_body_terms.emplace_back(-4.0 * Jy, "Sy", L - 1, "Sy", 0);
        m_two_body_terms.emplace_back(-4.0 * Jz, "Sz", L - 1, "Sz", 0);
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

void Heisenberg1D::print(std::ostream &stream) const
{
    SpinHalf1D::print(stream);
    stream << "\n\tJx:                 " << m_Jx;
    stream << "\n\tJy:                 " << m_Jy;
    stream << "\n\tJz:                 " << m_Jz;
    stream << "\n\thx:                 " << m_hx;
    stream << "\n\thy:                 " << m_hy;
    stream << "\n\thz:                 " << m_hz;
}