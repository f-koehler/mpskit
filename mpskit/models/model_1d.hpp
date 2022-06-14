#ifndef MPSKIT_MODELS_MODEL_1D
#define MPSKIT_MODELS_MODEL_1D

#include <itensor/mps/autompo.h>
#include <itensor/mps/mpo.h>
#include <itensor/mps/mps.h>
#include <itensor/mps/siteset.h>
#include <string>
#include <vector>

#include "../terms.hpp"
#include "../types.hpp"
#include "model.hpp"

struct OnePointFunction;
struct TwoPointFunction;

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

    void orderTerms();

    std::vector<OnePointFunction> generateOnePointFunctions(const std::string &op, const Real &prefactor = 1.0,
                                                            bool full = true) const;
    std::vector<TwoPointFunction> generateTwoPointFunctions(const std::string &op1, const std::string &op2,
                                                            const Real &prefactor = 1.0, bool full = false) const;

    itensor::MPO getHamiltonian() const override;
    virtual const std::vector<OneSiteTerm> &getOneBodyTerms() const;
    virtual const std::vector<TwoSiteTerm> &getTwoBodyTerms() const;
    bool isPeriodic() const;
    int getL() const;

    virtual void print(std::ostream &stream) const override;
};

#endif /* MPSKIT_MODELS_MODEL_1D */
