#ifndef DMRG_OBSERVER
#define DMRG_OBSERVER

#include <vector>
#include <itensor/mps/DMRGObserver.h>

class Observer : public itensor::DMRGObserver
{
private:
    std::vector<double> m_energies;

public:
    Observer(const itensor::MPS &psi, const itensor::Args &args = itensor::Args::global());

    virtual bool checkDone(const itensor::Args &args = itensor::Args::global());

    const std::vector<double> &getEnergies() const;
};

#endif /* DMRG_OBSERVER */
