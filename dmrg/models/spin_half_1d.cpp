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

auto SpinHalf1D::get_initial_state() const -> itensor::MPS
{
    return itensor::randomMPS(m_sites);
}

auto SpinHalf1D::get_total_sx_operator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sx", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::get_total_sy_operator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sy", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::get_total_sz_operator() const -> itensor::MPO
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sz", i;
    }
    return itensor::toMPO(ampo);
}

auto SpinHalf1D::get_observables() const -> std::map<std::string, Observable>
{
    auto result = Model1D::get_observables();
    result.insert({"Sx", Observable(get_total_sx_operator())});
    result.insert({"Sy", Observable(get_total_sy_operator())});
    result.insert({"Sz", Observable(get_total_sz_operator())});
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