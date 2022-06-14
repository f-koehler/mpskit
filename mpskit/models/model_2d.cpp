#include "model_2d.hpp"
#include <itensor/mps/autompo.h>

Model2D::Model2D(const itensor::SiteSet &sites, const itensor::LatticeGraph &lattice) : Model(sites), m_lattice(lattice)
{
}

auto Model2D::getHamiltonian() const -> itensor::MPO
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

auto Model2D::getOneBodyTerms() const -> const std::vector<OneSiteTerm> &
{
    return m_one_body_terms;
}

auto Model2D::getTwoBodyTerms() const -> const std::vector<TwoSiteTerm> &
{
    return m_two_body_terms;
}

void Model2D::print(std::ostream &stream) const
{
    Model::print(stream);
}