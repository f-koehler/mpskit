#ifndef DMRG_MODELS_MODEL
#define DMRG_MODELS_MODEL

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>
#include <map>
#include <string>

#include "../observable.hpp"

struct Observable;

class Model
{
  protected:
    itensor::SiteSet m_sites;

  public:
    explicit Model(itensor::SiteSet sites);
    virtual ~Model() = default;

    virtual itensor::MPS get_initial_state() const = 0;
    virtual itensor::MPO get_hamiltonian() const = 0;
    virtual std::map<std::string, Observable> get_observables() const;

    const itensor::SiteSet &getSites() const;
};

#endif /* DMRG_MODELS_MODEL */
