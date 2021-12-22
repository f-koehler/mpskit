#include "model.hpp"

Model::Model(const itensor::SiteSet &sites) : m_sites(sites)
{
}

std::map<std::string, Observable> Model::get_observables() const
{
    return {{"H", Observable(get_hamiltonian())}};
}

const itensor::SiteSet &Model::getSites() const
{
    return m_sites;
}