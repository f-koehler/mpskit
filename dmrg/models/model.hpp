#ifndef DMRG_MODELS_MODEL
#define DMRG_MODELS_MODEL

#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>

class Model
{
  protected:
    itensor::SiteSet m_sites;

  public:
    explicit Model(const itensor::SiteSet &sites);
    virtual ~Model() = default;

    virtual itensor::MPS get_initial_state() const = 0;
    virtual itensor::MPO get_hamiltonian() const = 0;

    const itensor::SiteSet &getSites() const;
};

#endif /* DMRG_MODELS_MODEL */
