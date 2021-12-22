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

bool SpinHalf1D::doesConserveSz() const
{
    return m_conserve_Sz;
}
bool SpinHalf1D::doesConserveParity() const
{
    return m_conserve_parity;
}