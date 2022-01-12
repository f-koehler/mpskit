#include "observer.hpp"

#include <algorithm>

Observer::Observer(const itensor::MPS &psi, std::shared_ptr<Model> model, const itensor::Args &args)
    : itensor::DMRGObserver(psi, args), m_observables(model->getObservables())
{
}

auto Observer::checkDone(const itensor::Args &args) -> bool
{
    const auto energy = args.getReal("Energy", 0.0);
    auto retval = DMRGObserver::checkDone(args);
    m_energies.push_back(energy);

    for (auto &[name, observable] : m_observables)
    {
        observable(psi());
        m_observable_values[name].push_back(observable.value);
        m_observable_squared_values[name].push_back(observable.squared);
        m_observable_variances[name].push_back(observable.variance);
    }

    return retval;
}

auto Observer::getEnergies() const -> const RealSeries &
{
    return m_energies;
}

const std::map<std::string, ComplexSeries> &Observer::getObservableValues() const
{
    return m_observable_values;
}
const std::map<std::string, ComplexSeries> &Observer::getObservableSquaredValues() const
{
    return m_observable_squared_values;
}
const std::map<std::string, ComplexSeries> &Observer::getObservableVariances() const
{
    return m_observable_variances;
}