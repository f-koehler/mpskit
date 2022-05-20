#ifndef MPSKIT_OBSERVER
#define MPSKIT_OBSERVER

#include <itensor/mps/DMRGObserver.h>
#include <itensor/mps/mps.h>
#include <itensor/util/args.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "models/model.hpp"
#include "observable.hpp"
#include "types.hpp"

class Model;

class Observer : public itensor::DMRGObserver
{
  private:
    RealSeries m_energies;
    IntSeries m_max_bond_dimensions;
    RealSeries m_average_bond_dimensions;

  public:
    explicit Observer(const itensor::MPS &psi, const itensor::Args &args = itensor::Args::global());

    virtual bool checkDone(const itensor::Args &args = itensor::Args::global());

    const RealSeries &getEnergies() const;
    const IntSeries &getMaxBondDimensions() const;
    const RealSeries &getAverageBondDimensions() const;
};

#endif /* MPSKIT_OBSERVER */
