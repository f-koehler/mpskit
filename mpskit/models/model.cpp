#include "model.hpp"

#include <utility>

#include "../observable.hpp"

struct OnePointFunction;
struct TwoPointFunction;

Model::Model(itensor::SiteSet sites) : m_sites(std::move(sites))
{
}

auto Model::getObservables() const -> std::map<std::string, Observable>
{
    return {{"H", Observable(getHamiltonian())}};
}

auto Model::getSites() const -> const itensor::SiteSet &
{
    return m_sites;
}

std::map<std::string, std::vector<OnePointFunction>> Model::getOnePointFunctions() const
{
    return {};
}

std::map<std::string, std::vector<TwoPointFunction>> Model::getTwoPointFunctions() const
{
    return {};
}

void Model::print(std::ostream &stream) const
{
    stream << "Model";
    // TODO: print model name
}

std::ostream &operator<<(std::ostream &stream, const Model &model)
{
    model.print(stream);
    return stream;
}