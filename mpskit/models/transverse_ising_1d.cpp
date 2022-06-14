#include "transverse_ising_1d.hpp"

#include <algorithm>
#include <cstddef>
#include <itensor/util/iterate.h>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "../json.hpp"
#include "../terms.hpp"
#include "../types.hpp"

TransverseIsing1D::TransverseIsing1D(int L, bool periodic, const Real &J, const Real &hx, const Real &hz)
    : SpinHalf1D(L, periodic, false, false), m_J(J), m_hx(hx), m_hz(hz)
{
    m_name = "TransverseIsing1D";

    m_one_body_terms.reserve(static_cast<std::size_t>(2 * L));
    m_two_body_terms.reserve(static_cast<std::size_t>(L));

    for (int i = 0; i < L; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * hz, "Sz", i);
    }

    for (int i = 0; i < L - 1; ++i)
    {
        m_two_body_terms.emplace_back(-4.0 * J, "Sz", i, "Sz", i + 1);
    }

    if (periodic)
    {
        m_two_body_terms.emplace_back(-4.0 * J, "Sz", L - 1, "Sz", 0);
    }
}

TransverseIsing1D::TransverseIsing1D(const json &js)
    : TransverseIsing1D(js["L"].get<int>(), jsonGetDefault<bool>(js, "periodic", false), js["J"].get<Real>(),
                        js["hx"].get<Real>(), js["hz"].get<Real>())
{
}

auto TransverseIsing1D::getJ() const -> const Real &
{
    return m_J;
}
auto TransverseIsing1D::getHx() const -> const Real &
{
    return m_hx;
}
auto TransverseIsing1D::getHz() const -> const Real &
{
    return m_hz;
}

void TransverseIsing1D::print(std::ostream &stream) const
{
    SpinHalf1D::print(stream);
    stream << "\n\tJ:                  " << m_J;
    stream << "\n\thx:                 " << m_hx;
    stream << "\n\thz:                 " << m_hz;
}