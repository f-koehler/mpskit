#include "terms.hpp"

#include <algorithm>
#include <itensor/mps/autompo.h>

OneSiteTerm::OneSiteTerm(const Real &prefactor, const std::string &op, int index)
    : m_prefactor(prefactor), m_op(op), m_index(index)
{
}

const Real &OneSiteTerm::getPrefactor() const
{
    return m_prefactor;
}

const std::string &OneSiteTerm::getOperator() const
{
    return m_op;
}

const int &OneSiteTerm::getIndex() const
{
    return m_index;
}

TwoSiteTerm::TwoSiteTerm(const Real &prefactor, const std::string &op1, int index1, const std::string &op2, int index2)
    : m_prefactor(prefactor), m_op1(op1), m_index1(index1), m_op2(op2), m_index2(index2)
{
}

void TwoSiteTerm::orderTerm()
{
    if (m_index1 == m_index2)
    {
        throw std::logic_error("Indices of TwoSiteTerm should be distinct");
    }
    if (m_index1 > m_index2)
    {
        std::swap(m_index1, m_index2);
        std::swap(m_op1, m_op2);
    }
}

const Real &TwoSiteTerm::getPrefactor() const
{
    return m_prefactor;
}
const std::string &TwoSiteTerm::getOperator1() const
{
    return m_op1;
}
const int &TwoSiteTerm::getIndex1() const
{
    return m_index1;
}
const std::string &TwoSiteTerm::getOperator2() const
{
    return m_op2;
}
const int &TwoSiteTerm::getIndex2() const
{
    return m_index2;
}

namespace itensor
{
auto operator+=(AutoMPO &lhs, const OneSiteTerm &term) -> AutoMPO &
{
    lhs += term.getPrefactor(), term.getOperator(), term.getIndex();
    return lhs;
}
auto operator+=(AutoMPO &lhs, const TwoSiteTerm &term) -> AutoMPO &
{
    lhs += term.getPrefactor(), term.getOperator1(), term.getIndex1(), term.getOperator2(), term.getIndex2();
    return lhs;
}
} // namespace itensor