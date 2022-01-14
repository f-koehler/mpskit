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
    : m_index1(index1), m_index2(index2), m_prefactor(prefactor), m_value(0.0)
{
    if (index1 == index2)
    {
        m_op = itensor::multSiteOps(itensor::op(sites, op1, index1 + 1), itensor::op(sites, op2, index1 + 1));
    }
    else
    {
        m_op = itensor::op(sites, op1, index1 + 1) * itensor::op(sites, op2, index2 + 1);
    }
}

const Complex &TwoPointFunction::getValue() const
{
    return m_value;
}

int TwoPointFunction::getIndex1() const
{
    return m_index1;
}

int TwoPointFunction::getIndex2() const
{
    return m_index2;
}

const Complex &TwoPointFunction::operator()(itensor::MPS &mps)
{
    itensor::ITensor ket, bra;
    if (m_index1 == m_index2)
    {
        ket = mps(m_index1 + 1);
    }
    else
    {
        ket = mps(m_index1 + 1) * mps(m_index2 + 1);
    }
    bra = itensor::dag(itensor::prime(ket, "Site"));
    m_value = m_prefactor * itensor::eltC(bra * m_op * ket);
    return m_value;
}