#include "observer.hpp"

Observer::Observer(const itensor::MPS &psi, const itensor::Args &args) : itensor::DMRGObserver(psi, args)
{
}

bool Observer::checkDone(const itensor::Args &args)
{
    const auto energy = args.getReal("Energy", 0.0);
    auto retval = DMRGObserver::checkDone(args);
    m_energies.push_back(energy);
    return retval;
}

const std::vector<double> &Observer::getEnergies() const
{
    return m_energies;
}