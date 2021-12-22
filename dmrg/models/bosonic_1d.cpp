#include "bosonic_1d.hpp"
#include <itensor/mps/sites/boson.h>

Bosonic1D::Bosonic1D(int L, bool periodic, bool conserve_N, int max_N)
    : Model1D(itensor::Boson(L, {"MaxOcc=", max_N, "ConserveNb=", conserve_N}), L, periodic), m_conserve_N(conserve_N),
      m_max_N(max_N)
{
}

itensor::MPS Bosonic1D::get_initial_state() const
{
    auto state = itensor::InitState(m_sites);
    state.set(1, std::to_string(m_max_N));
    return itensor::randomMPS(state);
}

bool Bosonic1D::doesConserveN() const
{
    return m_conserve_N;
}
int Bosonic1D::getMaxN() const
{
    return m_max_N;
}