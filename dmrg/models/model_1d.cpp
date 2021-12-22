#include "model_1d.hpp"

#include <itensor/mps/autompo.h>

#include "../terms.hpp"

Model1D::Model1D(const itensor::SiteSet &sites, int L, bool periodic) : Model(sites), m_L(L), m_periodic(periodic)
{
}

auto Model1D::get_hamiltonian() const -> itensor::MPO
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

auto Model1D::get_one_body_terms() const -> const std::vector<OneSiteTerm> &
{
    return m_one_body_terms;
}

auto Model1D::get_two_body_terms() const -> const std::vector<TwoSiteTerm> &
{
    return m_two_body_terms;
}

auto Model1D::isPeriodic() const -> bool
{
    return m_periodic;
}

auto Model1D::getL() const -> int
{
    return m_L;
}