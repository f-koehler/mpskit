#include "observer.hpp"

#include <utility>
#include <vector>

#include "models/model.hpp"

Observer::Observer(const itensor::MPS &psi, const itensor::Args &args) : itensor::DMRGObserver(psi, args)
{
}

auto Observer::checkDone(const itensor::Args &args) -> bool
{
    auto retval = DMRGObserver::checkDone(args);
    m_energies.push_back(args.getReal("Energy", 0));
    m_max_bond_dimensions.push_back(itensor::maxLinkDim(psi()));
    m_average_bond_dimensions.push_back(itensor::averageLinkDim(psi()));
    return retval;
}

const RealSeries &Observer::getEnergies() const
{
    return m_energies;
}
const IntSeries &Observer::getMaxBondDimensions() const
{
    return m_max_bond_dimensions;
}
const RealSeries &Observer::getAverageBondDimensions() const
{
    return m_average_bond_dimensions;
}
