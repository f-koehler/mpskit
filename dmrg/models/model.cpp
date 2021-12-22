#include "model.hpp"

Model::Model(const itensor::SiteSet &sites) : m_sites(sites)
{
}

const itensor::SiteSet &Model::getSites() const
{
    return m_sites;
}