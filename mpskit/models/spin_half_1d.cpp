#include "spin_half_1d.hpp"

#include <itensor/mps/autompo.h>
#include <itensor/mps/sites/spinhalf.h>
#include <itensor/util/iterate.h>

#include "../observable.hpp"

SpinHalf1D::SpinHalf1D(int L, bool periodic, bool conserve_Sz, bool conserve_parity)
    : Model1D(itensor::SpinHalf(L, {"ConserveSz=", conserve_Sz, "ConserveParity=", conserve_parity}), L, periodic),
      m_conserve_Sz(conserve_Sz), m_conserve_parity(conserve_parity)
{
}

auto SpinHalf1D::getInitialState() const -> itensor::MPS
{
    return itensor::randomMPS(m_sites);
}

auto SpinHalf1D::getTotalSxOperator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sx", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::getTotalSyOperator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sy", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::getTotalSzOperator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sz", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::getObservables() const -> std::map<std::string, Observable>
{
    auto result = Model1D::getObservables();
    result.insert({"Sx", Observable(getTotalSxOperator())});
    result.insert({"Sy", Observable(getTotalSyOperator())});
    result.insert({"Sz", Observable(getTotalSzOperator())});
    return result;
}

std::map<std::string, std::vector<OnePointFunction>> SpinHalf1D::getOnePointFunctions() const
{
    std::map<std::string, std::vector<OnePointFunction>> result;
    result.insert({"sx", generateOnePointFunctions("Sx", 2.0)});
    result.insert({"sy", generateOnePointFunctions("Sy", 2.0)});
    result.insert({"sz", generateOnePointFunctions("Sz", 2.0)});
    return result;
}

std::map<std::string, std::vector<TwoPointFunction>> SpinHalf1D::getTwoPointFunctions() const
{
    std::map<std::string, std::vector<TwoPointFunction>> result;
    result.insert({"sx_sx", generateTwoPointFunctions("Sx", "Sx", 4.0, m_L <= 32)});
    result.insert({"sy_sy", generateTwoPointFunctions("Sy", "Sy", 4.0, m_L <= 32)});
    result.insert({"sz_sz", generateTwoPointFunctions("Sz", "Sz", 4.0, m_L <= 32)});
    return result;
}

auto SpinHalf1D::doesConserveSz() const -> bool
{
    return m_conserve_Sz;
}
auto SpinHalf1D::doesConserveParity() const -> bool
{
    return m_conserve_parity;
}