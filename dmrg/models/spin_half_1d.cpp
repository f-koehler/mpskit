#include "spin_half_1d.hpp"
#include <itensor/mps/sites/spinhalf.h>

SpinHalf1D::SpinHalf1D(int L, bool periodic, bool conserve_Sz, bool conserve_parity)
    : Model1D(itensor::SpinHalf(L, {"ConserveSz=", conserve_Sz, "ConserveParity=", conserve_parity}), L, periodic),
      m_conserve_Sz(conserve_Sz), m_conserve_parity(conserve_parity)
{
}

itensor::MPS SpinHalf1D::get_initial_state() const
{
    return itensor::randomMPS(m_sites);
}

itensor::MPO SpinHalf1D::get_total_sx_operator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sx", i;
    }
    return itensor::toMPO(ampo);
}

itensor::MPO SpinHalf1D::get_total_sy_operator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sy", i;
    }
    return itensor::toMPO(ampo);
}

itensor::MPO SpinHalf1D::get_total_sz_operator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_L))
    {
        ampo += 2.0, "Sz", i;
    }
    return itensor::toMPO(ampo);
}

std::map<std::string, Observable> SpinHalf1D::get_observables() const
{
    auto result = Model1D::get_observables();
    result.insert({"Sx", Observable(get_total_sx_operator())});
    result.insert({"Sy", Observable(get_total_sy_operator())});
    result.insert({"Sz", Observable(get_total_sz_operator())});
    return result;
}

bool SpinHalf1D::doesConserveSz() const
{
    return m_conserve_Sz;
}
bool SpinHalf1D::doesConserveParity() const
{
    return m_conserve_parity;
}