#ifndef DMRG_MODELS_MODEL1D
#define DMRG_MODELS_MODEL1D

#include "../terms.hpp"
#include "../types.hpp"
#include "model.hpp"
#include <itensor/mps/autompo.h>
#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>
#include <vector>

class Model1D : public Model
{
  protected:
    int m_L;
    bool m_periodic;
    std::vector<OneSiteTerm> m_one_body_terms;
    std::vector<TwoSiteTerm> m_two_body_terms;

  public:
    explicit Model1D(const itensor::SiteSet &sites, int L, bool periodic);
    virtual ~Model1D() = default;

    itensor::MPO get_hamiltonian() const override;
    virtual const std::vector<OneSiteTerm> &get_one_body_terms() const;
    virtual const std::vector<TwoSiteTerm> &get_two_body_terms() const;

    bool isPeriodic() const;
    int getL() const;
};

#endif /* DMRG_MODELS_MODEL1D */
