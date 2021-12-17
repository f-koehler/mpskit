#include "observer.hpp"

#include <algorithm>

Observer::Observer(const itensor::MPS &psi, const itensor::Args &args) : itensor::DMRGObserver(psi, args)
{
}

auto Observer::checkDone(const itensor::Args &args) -> bool
{
    const auto energy = args.getReal("Energy", 0.0);
    auto retval = DMRGObserver::checkDone(args);
    m_energies.push_back(energy);
    return retval;
}

auto Observer::getEnergies() const -> const std::vector<double> &
{
    return m_energies;
}