#include "spin_half_square.hpp"

#include <itensor/mps/autompo.h>
#include <itensor/mps/lattice/square.h>
#include <itensor/mps/sites/spinhalf.h>
#include <itensor/util/iterate.h>

SpinHalfSquare::SpinHalfSquare(int Lx, int Ly, bool conserve_Sz, bool conserve_parity)
    : Model2D(itensor::SpinHalf(Lx * Ly, {"ConserveSz=", conserve_Sz, "ConserveParity=", conserve_parity}),
              itensor::squareLattice(Lx, Ly, {"YPeriodic=", false})),
      m_conserve_Sz(conserve_Sz), m_conserve_parity(conserve_parity), m_Lx(Lx), m_Ly(Ly)
{
}

itensor::MPS SpinHalfSquare::getInitialState() const
{
    return itensor::randomMPS(m_sites);
}

itensor::MPO SpinHalfSquare::getTotalSxOperator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_Lx * m_Ly))
    {
        ampo += 2.0, "Sx", i;
    }
    return itensor::toMPO(ampo);
}
itensor::MPO SpinHalfSquare::getTotalSyOperator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_Lx * m_Ly))
    {
        ampo += 2.0, "Sy", i;
    }
    return itensor::toMPO(ampo);
}
itensor::MPO SpinHalfSquare::getTotalSzOperator() const
{
    itensor::AutoMPO ampo(m_sites);
    for (int i : itensor::range1(m_Lx * m_Ly))
    {
        ampo += 2.0, "Sz", i;
    }
    return itensor::toMPO(ampo);
}
std::map<std::string, Observable> SpinHalfSquare::getObservables() const
{
    auto result = Model2D::getObservables();
    result.insert({"Sx", Observable(getTotalSxOperator())});
    result.insert({"Sy", Observable(getTotalSyOperator())});
    result.insert({"Sz", Observable(getTotalSzOperator())});
    return result;
}

bool SpinHalfSquare::doesConserveSz() const
{
    return m_conserve_Sz;
}
bool SpinHalfSquare::doesConserveParity() const
{
    return m_conserve_parity;
}