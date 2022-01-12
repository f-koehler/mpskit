#ifndef MPSKIT_OBSERVER
#define MPSKIT_OBSERVER

#include "models/model.hpp"
#include "observable.hpp"
#include "types.hpp"

#include <itensor/mps/DMRGObserver.h>
#include <itensor/mps/mps.h>
#include <itensor/util/args.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Observer : public itensor::DMRGObserver
{
  private:
    RealSeries m_energies;
    std::map<std::string, Observable> m_observables;
    std::map<std::string, ComplexSeries> m_observable_values;
    std::map<std::string, ComplexSeries> m_observable_squared_values;
    std::map<std::string, ComplexSeries> m_observable_variances;

  public:
    explicit Observer(const itensor::MPS &psi, std::shared_ptr<Model> model,
                      const itensor::Args &args = itensor::Args::global());

    virtual bool checkDone(const itensor::Args &args = itensor::Args::global());

    const RealSeries &getEnergies() const;
    const std::map<std::string, ComplexSeries> &getObservableValues() const;
    const std::map<std::string, ComplexSeries> &getObservableSquaredValues() const;
    const std::map<std::string, ComplexSeries> &getObservableVariances() const;
};

#endif /* MPSKIT_OBSERVER */
