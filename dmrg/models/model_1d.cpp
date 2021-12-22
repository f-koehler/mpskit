#include "model_1d.hpp"

Model1D::Model1D(const itensor::SiteSet &sites, int L, bool periodic) : Model(sites), m_L(L), m_periodic(periodic)
{
}

itensor::MPO Model1D::get_hamiltonian() const
{
    itensor::AutoMPO ampo(m_sites);
    for (const auto &term : m_one_body_terms)
    {
        ampo += term;
    }
    for (const auto &term : m_two_body_terms)
    {
        ampo += term;
    }
    return itensor::toMPO(ampo);
}

const std::vector<OneSiteTerm> &Model1D::get_one_body_terms() const
{
    return m_one_body_terms;
}

const std::vector<TwoSiteTerm> &Model1D::get_two_body_terms() const
{
    return m_two_body_terms;
}

bool Model1D::isPeriodic() const
{
    return m_periodic;
}

int Model1D::getL() const
{
    return m_L;
}