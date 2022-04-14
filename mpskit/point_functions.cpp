#include "point_functions.hpp"

#include <complex>
#include <itensor/itensor_impl.h>
#include <itensor/mps/mps.h>
#include <itensor/types.h>

OnePointFunction::OnePointFunction(const itensor::SiteSet &sites, int index, const std::string &op, Real prefactor)
    : m_index(index), m_prefactor(prefactor), m_value(0.0), m_op(itensor::op(sites, op, index + 1))
{
}

const Complex &OnePointFunction::getValue() const
{
    return m_value;
}

int OnePointFunction::getIndex() const
{
    return m_index;
}

const Complex &OnePointFunction::operator()(itensor::MPS &mps)
{
    mps.position(m_index + 1);
    auto ket = mps(m_index + 1);
    auto bra = itensor::dag(itensor::prime(ket, "Site"));
    m_value = m_prefactor * itensor::eltC(bra * m_op * ket);
    return m_value;
}

TwoPointFunction::TwoPointFunction(const itensor::SiteSet &sites, int index1, int index2, const std::string &op1,
                                   const std::string &op2, Real prefactor)
{
    m_prefactor = prefactor;
    m_value = 0.0;

    if (index1 < index2)
    {
        m_ordered = true;
        m_same_index = false;
        m_i = index1 + 1;
        m_j = index2 + 1;
        m_op_i = itensor::op(sites, op1, m_i);
        m_op_j = itensor::op(sites, op2, m_j);
    }
    else if (index1 == index2)
    {
        m_ordered = false;
        m_same_index = true;
        m_i = index1 + 1;
        m_j = index1 + 1;
        m_op_i = itensor::op(sites, op1, m_i);
        m_op_j = itensor::op(sites, op2, m_j);
    }
    else
    {
        m_ordered = false;
        m_same_index = false;
        m_i = index2 + 1;
        m_j = index1 + 1;
        m_op_i = itensor::op(sites, op2, m_i);
        m_op_j = itensor::op(sites, op1, m_j);
    }
}

const Complex &TwoPointFunction::getValue() const
{
    return m_value;
}

int TwoPointFunction::getIndex1() const
{
    if (m_ordered)
    {
        return m_i - 1;
    }
    else
    {
        return m_j - 1;
    }
}

int TwoPointFunction::getIndex2() const
{
    if (m_ordered)
    {
        return m_j - 1;
    }
    else
    {
        return m_i - 1;
    }
}

const Complex &TwoPointFunction::operator()(itensor::MPS &psi)
{
    psi.position(m_i);

    if (m_same_index)
    {
        auto ket = psi(m_i);
        auto bra = itensor::dag(itensor::prime(ket, "Site"));
        m_value = itensor::eltC(bra * itensor::multSiteOps(m_op_i, m_op_j) * ket);
        return m_value;
    }

    auto psidag = itensor::dag(psi);
    psidag.prime("Link");

    auto li_1 = itensor::leftLinkIndex(psi, m_i);

    auto C = itensor::prime(psi(m_i), li_1) * m_op_i;
    C *= itensor::prime(psidag(m_i), "Site");

    for (int k = m_i + 1; k < m_j; ++k)
    {
        C *= psi(k);
        C *= psidag(k);
    }

    auto lj = itensor::rightLinkIndex(psi, m_j);

    C *= itensor::prime(psi(m_j), lj) * m_op_j;
    C *= itensor::prime(psidag(m_j), "Site");

    m_value = itensor::eltC(C);
    if (!m_ordered)
    {
        m_value = std::conj(m_value);
    }
    return m_value;
}