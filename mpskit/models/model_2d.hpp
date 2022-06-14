#ifndef MPSKIT_MODELS_MODEL_SQUARE
#define MPSKIT_MODELS_MODEL_SQUARE

#include <itensor/mps/lattice/latticebond.h>
#include <itensor/mps/siteset.h>

#include "../terms.hpp"
#include "model.hpp"

class Model2D : public Model
{
  protected:
    itensor::LatticeGraph m_lattice;
    std::vector<OneSiteTerm> m_one_body_terms;
    std::vector<TwoSiteTerm> m_two_body_terms;

  public:
    explicit Model2D(const itensor::SiteSet &sites, const itensor::LatticeGraph &lattice);
    virtual ~Model2D() = default;

    itensor::MPO getHamiltonian() const override;
    virtual const std::vector<OneSiteTerm> &getOneBodyTerms() const;
    virtual const std::vector<TwoSiteTerm> &getTwoBodyTerms() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_MODEL_SQUARE */
