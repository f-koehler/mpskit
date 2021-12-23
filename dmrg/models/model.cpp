#include "model.hpp"

#include <utility>

#include "../observable.hpp"

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

std::map<std::string, std::vector<OnePointFunction>> Model::get_one_point_functions() const
{
    return {};
}

std::map<std::string, std::vector<TwoPointFunction>> Model::get_two_point_functions() const
{
    return {};
}