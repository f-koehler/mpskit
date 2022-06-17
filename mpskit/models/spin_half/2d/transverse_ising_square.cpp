#include "transverse_ising_square.hpp"

TransverseIsingSquare::TransverseIsingSquare(int Lx, int Ly, const Real &J, const Real &hx, const Real &hz)
    : SpinHalfSquare(Lx, Ly, false, false), m_J(J), m_hx(hx), m_hz(hz)
{
    m_name = "TransverseIsingSquare";

    m_one_body_terms.reserve(static_cast<std::size_t>(2 * Lx * Ly));
    m_two_body_terms.reserve(static_cast<std::size_t>(m_lattice.size()));

    for (int i = 0; i < Lx * Ly; ++i)
    {
        m_one_body_terms.emplace_back(-2.0 * hx, "Sx", i);
        m_one_body_terms.emplace_back(-2.0 * hz, "Sz", i);
    }

    for (const auto &bond : m_lattice)
    {
        m_two_body_terms.emplace_back(-4.0 * J, "Sz", bond.s1 - 1, "Sz", bond.s2 - 1);
    }
}
TransverseIsingSquare::TransverseIsingSquare(const json &js)
    : TransverseIsingSquare(js["Lx"].get<int>(), js["Ly"].get<int>(), js["J"].get<Real>(), js["hx"].get<Real>(),
                            js["hz"].get<Real>())
{
}

const Real &TransverseIsingSquare::getJ() const
{
    return m_J;
}
const Real &TransverseIsingSquare::getHx() const
{
    return m_hx;
}
const Real &TransverseIsingSquare::getHz() const
{
    return m_hz;
}

void TransverseIsingSquare::print(std::ostream &stream) const
{
    SpinHalfSquare::print(stream);
    stream << "\n\tJ:                  " << m_J;
    stream << "\n\thx:                 " << m_hx;
    stream << "\n\thz:                 " << m_hz;
}