#ifndef MPSKIT_OBSERVER
#define MPSKIT_OBSERVER

#include <itensor/mps/DMRGObserver.h>
#include <itensor/mps/mps.h>
#include <itensor/util/args.h>
#include <vector>

class Observer : public itensor::DMRGObserver
{
  private:
    std::vector<double> m_energies;

  public:
    explicit Observer(const itensor::MPS &psi, const itensor::Args &args = itensor::Args::global());

    virtual bool checkDone(const itensor::Args &args = itensor::Args::global());

    const std::vector<double> &getEnergies() const;
};

#endif /* MPSKIT_OBSERVER */
