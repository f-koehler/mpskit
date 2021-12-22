#include "model.hpp"

#include <utility>

#include "/home/fkoehler/code/ising/dependencies/dmrg/dmrg/observable.hpp"

Model::Model(itensor::SiteSet sites) : m_sites(std::move(sites))
{
}

auto Model::get_observables() const -> std::map<std::string, Observable>
{
    return {{"H", Observable(get_hamiltonian())}};
}

auto Model::getSites() const -> const itensor::SiteSet &
{
    return m_sites;
}